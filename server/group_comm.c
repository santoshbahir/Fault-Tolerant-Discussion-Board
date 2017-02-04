#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "group_comm.h"
#include "replication.h"
#include "handleclients.h"
#include "debugmsg.h"
#include "sync.h"


void connect_spread(){
    int ret;

    sp_time test_timeout;
    test_timeout.sec = 5;
    test_timeout.usec = 0;

    /*SPREAD CONNECTION: KARACHI..!*/
    sprintf(Spread_name, "22222@localhost");
    ret = SP_connect_timeout( Spread_name, User, 0, 1, &Mbox, Private_group, test_timeout );
    if( ret != ACCEPT_SESSION )
    {
        SP_error(ret);
        Bye();
    }
    printf("User: connected to %s with private group %s\n", Spread_name, Private_group );
    E_init();
}


void join_group(const char *group_name){
    int ret;

    ret = SP_join(Mbox, group_name);
    if( ret < 0 )
        SP_error(ret);
    else
        printf("Server joined %s group\n",group_name);

    return;
}


void create_own_group_name(int server_id)
{
    char extn[]="db_server";
    sprintf(server_public_group,"%d_",server_id);
    strcat(server_public_group, extn);

    return;
}


void read_message()
{
    struct spread_packet    mess;
    char                    target_groups[MAX_MEMBERS][MAX_GROUP_NAME];
    membership_info         memb_info;
    int                     service_type;
    int                     num_groups;
    int16                   mess_type;
    int                     endian_mismatch;
    int                     ret;

    service_type = 0;

    ret = SP_receive( 
                        Mbox, 
                        &service_type, 
                        sender, 100, 
                        &num_groups, 
                        target_groups,
                        &mess_type, 
                        &endian_mismatch, 
                        sizeof(mess), 
                        (char *)&mess 
                    );
    //  printf("\n============================\n");
    if(ret < 0)
    {
        if((ret == GROUPS_TOO_SHORT) || (ret == BUFFER_TOO_SHORT)){
            service_type = DROP_RECV;
            printf("\n========Buffers or Groups too Short=======\n");
            ret = SP_receive(
                                Mbox, 
                                &service_type, 
                                sender, 
                                MAX_MEMBERS, 
                                &num_groups, 
                                target_groups,
                                &mess_type, 
                                &endian_mismatch, 
                                sizeof(mess), 
                                (char *)&mess );
        }
    }

    if (ret < 0 )
    {
        if( ! To_exit )
        {
            SP_error( ret );
            printf("\n============================\n");
            printf("\nBye.\n");
        }
        exit( 0 );
    }

    bool updateApplied=false;
    int server_index;

    if(Is_regular_mess( service_type ))
    {
//        printf("Packet Type=%d\n", mess.packet_type);
        switch(mess.packet_type)
        {
            case packet_Client:
                handle_client_req(mess);
                break;
            case packet_Update:

                PDEBUG_GROUP_COMM("Received update message\n");
                
                server_index=(((struct msg_update *)&mess)->update.server_id);
                if(!syncOn){
                PDEBUG_GROUP_COMM("Received update message\n");
                    if(my_server_id != server_index)
                    {
                        updateApplied=apply_update(mess);
                PDEBUG_GROUP_COMM("Received update message\n");
                    }
                
                PDEBUG_GROUP_COMM("Received update message\n");
                    send_response(&(((struct msg_update *)&mess)->update));
                }
                break;

            case packet_Vector:
                process_vector(mess);
                break;

            case packet_SyncUpdate:
                updateApplied=apply_update(mess);

                if(updateApplied){
                    send_response(&(((struct msg_update *)&mess)->update));
                }
                break;

            case packet_SyncEnd:
                process_syncEnd(mess);
                break;

            default:
                printf("Unknown message\n");
                exit(0);
        }
        
    }

    /*Partition Handling*/
    else if(Is_membership_mess(service_type))
        {
            ret = SP_get_memb_info( 
                                        (char *)&mess, 
                                        service_type, 
                                        &memb_info 
                                   );

            if (ret < 0) {
                printf("BUG: membership message does not have valid body\n");
                SP_error(ret);
                exit(1);
            }

        if(Is_reg_memb_mess( service_type)){

            bool clientLeft=false;        

            printf("Received REGULAR membership for group %s with %d members," 
                   "where I am member %d:\n",sender, num_groups, mess_type);

            if( Is_caused_join_mess( service_type ) )
            {
                printf("Due to the JOIN of %s\n", memb_info.changed_member );
            }else if( Is_caused_leave_mess( service_type ) ){
                printf("Due to the LEAVE of %s\n", memb_info.changed_member );
    

                // if the group is not replication group and 
                // sender is not me (my private group)
                if((strcmp(sender, rs_group) != 0) &&
                    (strcmp(Private_group, memb_info.changed_member) != 0))
                    clientLeft=true;

            }else if( Is_caused_disconnect_mess( service_type ) ){
                printf("Due to the DISCONNECT of %s\n", memb_info.changed_member );

                //if(strcmp(sender, rs_group))
                if((strcmp(sender, rs_group) != 0) &&
                    (strcmp(Private_group, memb_info.changed_member) != 0))
                    clientLeft=true;

            }

            /*  ALGORITHM:-
             *      -   IF the left group is not replication group; then search
             *      the client on client list/dboard list and remove it from it
             */

            if(clientLeft){
                clientLeft=false;

                remove_client(memb_info.changed_member);
            }

        }
        
        // if client leaves the group
        // remove the client from client list or from db-client list

        // if server himself leaves his own well-known group, the do nothing


        // if server join/leaves the replication-group; then start sync
        // Start Synchronization

        
        if(!strcmp(sender, rs_group)){
            num_memb=num_groups;
            // create my view
            
            if( num_memb > 1)
                sync();
            else{
                int i;

                for(i=0; i < NUM_OF_SERVERS; i++){
                    if(my_server_id == i+1){
                        server_view[i]=true;
                    }
                    else{
                        server_view[i]=false;
                    }
                }
            }
        }
    }
    else
        printf("received message of unknown message type 0x%x with ret %d\n", service_type, ret);

    fflush(stdout);
}

void Bye()
{
    To_exit = 1;

    printf("\nBye.\n");

    exit(0);
}

