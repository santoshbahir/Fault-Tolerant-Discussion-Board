#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sync.h"
#include "typedefine.h"
#include "replication.h"
#include "group_comm.h"
#include "handleclients.h"
#include "file.h"
#include "queue.h"

void send_server_updates(int server_id, int min_lts, int max_lts);
void print_my_job(int *server_id, int *min_lts, int *max_lts);
void print_summary(int **summary);

void sync()
{
    /*
     * - Turn on the syncOn
     * - Exchange lts-vector
     * - wait until all the vectors received
     *   If(all the vectors received)
     *      - find the minimum and maximum I can send
     *      - find If younger server can send
     * - Once all the updates are sent syncEnd message     
     */ 

    int i,j;
    int ret;

    // turn on the syncronization
    syncOn=true;
    vector_rcvd=0;
    syncEnd_rcvd=0;

    /*Create the space for each vector from all the servers*/
    if( (summary=(int **)malloc(NUM_OF_SERVERS*sizeof(int))) == NULL ){
        printf("Malloc failed ... exiting\n");
        exit(0);
    }

    for(i=0; i<NUM_OF_SERVERS; i++){
        if((summary[i]=(int *)malloc(NUM_OF_SERVERS*sizeof(int))) == NULL){
            printf("Malloc failed ... exiting\n");
            exit(0);
        }
    }

    // initialize the summary
    for(i=0; i<NUM_OF_SERVERS; i++){
        for(j=0; j<NUM_OF_SERVERS; j++){
            summary[i][i]=0;
        }
    }

    // Make the view member false for all servers

    for(i=0; i<NUM_OF_SERVERS; i++){
        server_view[i]=false;
    }

    // Build the vector messages

    struct msg_vector vector_msg;

    vector_msg.packet_type=packet_Vector;
    vector_msg.server_index=my_server_id;
    memcpy(vector_msg.server_vector, s_mdata.server_lts, (NUM_OF_SERVERS * sizeof(int)));
    
    // send the vector message of mine
    ret = SP_multicast(
                        Mbox,
                        AGREED_MESS,
                        rs_group,
                        1,
                        sizeof(struct msg_vector),
                        (char *)&(vector_msg)
                    ); 

    if( ret < 0 )
    {
        SP_error( ret );
        Bye();
    }

    return;
}


void process_vector(struct spread_packet vector_msg)
{
    int ret;

    struct msg_vector *msg_vector;

    msg_vector=(struct msg_vector *)&vector_msg;

    print_vector(msg_vector);
    
    vector_rcvd=vector_rcvd+1; 

    memcpy(summary[(msg_vector->server_index)-1],\
                    msg_vector->server_vector, NUM_OF_SERVERS * sizeof(int));
    
    printf("\nvector_rcvd=%d\tnum_groups=%d\n",vector_rcvd, num_memb);

    // update the view as vectors are received
    server_view[(msg_vector->server_index)-1]=true;
    
    print_server_info();

    if(vector_rcvd==num_memb){
        send_updates();
        // Send endSync message

        struct spread_packet syncEnd;
        syncEnd.packet_type=packet_SyncEnd;        
        ret = SP_multicast(
                            Mbox,
                            AGREED_MESS,
                            rs_group,
                            1,
                            sizeof(struct spread_packet),
                            (char *)&(syncEnd)
                        );
       if( ret < 0 )
        {
            SP_error( ret );
            Bye();
        }

        // once all the endSync received; turn of sync
    }

    return;
}

void send_updates()
{
    int server_id[NUM_OF_SERVERS];     //  I will send message of this servers
    int min_lts[NUM_OF_SERVERS];        //  From this lts I will start
    int max_lts[NUM_OF_SERVERS];        //  Till this lts I will send

    int i;
    int j; //SSB
    bool shouldSend=true; //SSB
    
    find_my_job(server_id, min_lts, max_lts);
    print_my_job(server_id, min_lts, max_lts);

    for(i=1; i<=NUM_OF_SERVERS; i++){
        if(server_id[i-1]==my_server_id){
            //SSB:IF my younger server has data it will send
            shouldSend=true;
            for(j=i-2; j>=0; j--){
                if(max_lts[j]==max_lts[i-1])
                    shouldSend=false;
            }
            if(shouldSend)
                send_server_updates(i, min_lts[i-1], max_lts[i-1]);
        }
    }

    return;
}


void send_server_updates(int server_id, int min_lts, int max_lts)
{
    /*
     *  search the log for <server_id, min_lts to max_lts and send it>
     */
    
    int i;   
    int ret;
    struct msg_update update_msg; 
    off_t offset;

    update_msg.packet_type=packet_SyncUpdate;

    for(i = min_lts + 1; i <= max_lts; i++){
        if((offset=search_log(i, server_id, &(update_msg.update)))==-1){
            printf("Update record should have present...\n");
//            exit(0);
        }
        else{
            print_update(&(update_msg.update));
            // Somebody needs this message; send it
            ret = SP_multicast(
                                Mbox,
                                AGREED_MESS,
                                rs_group,
                                1,
                                sizeof(struct msg_update),
                                (char *)&(update_msg)
                            );

            if( ret < 0 )
            {
                SP_error( ret );
                Bye();
            }
        }
    }
    
    return;
}


void process_syncEnd(struct spread_packet syncEnd)
{

    int i;

    syncEnd_rcvd=syncEnd_rcvd+1; 

    if(syncEnd_rcvd==num_memb){
        print_summary(summary);


    print_queue();
    
        syncOn=false;
        for(i=0; i<NUM_OF_SERVERS; i++){
    //        free(summary[i]);
        }

        // Here process the enqueued request during sync process

        struct request *req_cr;
        while(!queueEmpty()){
            req_cr = dequeueReq();
            
            memcpy(sender, req_cr->client, MAX_GROUP_NAME);
            handle_client_req(*((struct spread_packet *)&(req_cr->req)));
            printf("Processing done\n");
        }
    }

    return;
}

void find_my_job(int *server_id, int *min_lts, int *max_lts)
{
    int i;

    int max_server_id=-1;
    int max=-1;
    int min=-1;

    
    for(i=0; i<NUM_OF_SERVERS; i++){
        find_max_min(i, &max_server_id, &max, &min);
        server_id[i]=max_server_id;
        max_lts[i]=max;
        min_lts[i]=min;
    } 

    return;
}


void find_max_min(int index, int *m_id, int *max_lts, int *min_lts)
{
    int i;
    int tmp_min=1000000;
    int tmp_max=0;
    int tmp_mach_id=1;

    
    /*
     * Comment the logic very important
     */ 
    for(i=0; i<NUM_OF_SERVERS; i++){
        if((summary[i][index] < tmp_min) && server_view[i]){
            tmp_min=summary[i][index];
        }
        if(summary[i][index] > tmp_max && server_view[i]){
            tmp_max=summary[i][index];
            tmp_mach_id=i+1;
        }
    }

    *m_id       =   tmp_mach_id;
    *max_lts    =   tmp_max;
    *min_lts    =   tmp_min;

    return;
}

void print_vector(struct msg_vector *vector_msg)
{
    int i;
    printf("\n------------------------------------------------------------------\n");
    printf("\nVECTOR MESSAGE -->\n");
    printf("\tserver_index : %d\n", vector_msg->server_index);

    printf("\tvector       = [");
    for(i=1; i<=NUM_OF_SERVERS; i++){
        if(i==NUM_OF_SERVERS)
            printf("%d:%d", i, vector_msg->server_vector[i-1]);
        else
            printf("%d:%d\t", i, vector_msg->server_vector[i-1]);
    }
    printf("]\n\n------------------------------------------------------------------\n");

    return;
}


void print_my_job(int *server_id, int *min_lts, int *max_lts)
{
    int i;
    printf("\n------------------------------------------------------------------\n");
    printf("\nMY KNOWLEDGE:- \n");

    printf("\tsender       = [");
    for(i=1; i<=NUM_OF_SERVERS; i++){
        if(i==NUM_OF_SERVERS)
            printf("%d]\n", server_id[i-1]);
        else
            printf("%d\t", server_id[i-1]);
    }

    printf("\tmin_lts      = [");
    for(i=1; i<=NUM_OF_SERVERS; i++){
        if(i==NUM_OF_SERVERS)
            printf("%d]\n", min_lts[i-1]);
        else
            printf("%d\t", min_lts[i-1]);
    }

    printf("\tmax_lts      = [");
    for(i=1; i<=NUM_OF_SERVERS; i++){
        if(i==NUM_OF_SERVERS)
            printf("%d]\n", max_lts[i-1]);
        else
            printf("%d\t", max_lts[i-1]);
    }
    printf("\n\n------------------------------------------------------------------\n");

    return;
}

void print_summary(int **summary)
{
    int i,j;
    printf("\n------------------------------------------------------------------\n");
    printf("\nSUMMARY:- \n");

    for(i=0; i<NUM_OF_SERVERS; i++){
        for(j=0; j<NUM_OF_SERVERS; j++){
            if(j+1==NUM_OF_SERVERS)
                printf("%d\n", summary[i][j]);
            else
                printf("%d\t", summary[i][j]);
        }
    }

    printf("\n\n------------------------------------------------------------------\n");
    return;
}
