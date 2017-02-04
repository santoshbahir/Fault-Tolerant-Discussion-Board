#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sp.h"
#include "typedefine.h"
#include "replication.h"
#include "group_comm.h"
#include "mem_alloc.h"
#include "file.h"
#include "list.h"
#include "sync.h"
#include "queue.h"
#include "debugmsg.h"
#include "handleclients.h"

void send_dboard(struct db_node *db);
void leave_db();
void join_dboard();

void handle_client_req(struct spread_packet client_request)
{
    struct req_client *ptr_client_req=CASTTO_REQ_CLIENT(&client_request);

    // Enqueue the request if sync is on. 
    // Remember to put the client private group name in it

    if(syncOn){
        struct request *cl_req=alloc_request();

        memcpy(cl_req->client, sender, MAX_GROUP_NAME);
        cl_req->req = *ptr_client_req;

        enqueueReq(cl_req);
    }

    PDEBUG_HANDLECLIENTS("op=%c\n",ptr_client_req->op);
    switch(ptr_client_req->op)
    {   
        case 'c':
            print_request(ptr_client_req);
            join_group(ptr_client_req->obj);
            put_on_client_list(ptr_client_req);
            break;
        case 'j':
            print_request(ptr_client_req);
            join_db(ptr_client_req);
            break;
        case 'a':
            print_request(ptr_client_req);
            update_replicas(ptr_client_req);
            break;
        case 'd':
            update_replicas(ptr_client_req);
            print_request(ptr_client_req);
            break;
        case 'e':
            print_request(ptr_client_req);
            update_replicas(ptr_client_req);
            break;
        case 'v':
            print_request(ptr_client_req);
            send_view();
            break;
        default:
            print_request(ptr_client_req);
            PDEBUG_HANDLECLIENTS("Unknown operation is received\n");
    }   
    return;
}

void put_on_client_list(const struct req_client *ptr_client_req)
{
    struct ll_node *ll_client=alloc_ll_node();
    struct client_node *client=alloc_client_node();

    memcpy( client->client_server_group, \
                    ptr_client_req->obj,CS_GROUP_NAME_SIZE);
    memcpy( client->client_private_group, \
                    sender,MAX_GROUP_NAME);

    ll_client->data=(struct client_node *)client;
    add(&client_list_head, ll_client);

    return;
}


void join_db(const struct req_client *ptr_cr)
{
    struct ll_node *ll_db;
    struct db_node *db;

    struct ll_node *ll_client;

//    struct obj_record *obj_rec=alloc_obj_record();
    
    /*
     * if -1 send that means this is first request so first join request
     * else it is switching the db and obj will contain some value and below if
     * condition will be true as -1 != old db value
     */
   
    if(strcmp(ptr_cr->obj, "-1")){ // this is switch command obj contains old db-name
        if((ll_db=search_db(&db_list_head, ptr_cr->obj))==NULL){
            printf("Old db is not present...exiting\n");
            exit(0);
        }
        else{
            db=(struct db_node *)(ll_db->data);

            if((ll_client=search_client(&(db->cl_head), sender))==NULL){
                printf("client is not present on old db...exiting\n");
                exit(0);            
            }
            else{
                 // remove client from old db-node
                rem(&(db->cl_head),ll_client);
                db->total_clients=db->total_clients-1;
                
                // get new db-node if not present create new one
                if((ll_db=search_db(&db_list_head, ptr_cr->db_name))==NULL){
                    PDEBUG_HANDLECLIENTS("new db is not present in memory...Create one \n");

                    ll_db=alloc_ll_node();
                    db=alloc_db_node();

                    get_db(ptr_cr->db_name, &(db->db_rec));

                    db->cl_head=NULL;
                    db->total_clients=0;

                    ll_db->data=(void *)db;
                    add(&db_list_head, ll_db);
                }
                else{
                    db=(struct db_node *)(ll_db->data);
                }

                // put client on new db-node
                add(&(db->cl_head), ll_client);
                db->total_clients=db->total_clients+1;
                // send dboard to clients
                send_dboard(db);
            }
        }
    }
    else{    // This is first time joing of the request
        // get new db-node if not present create new one
        if((ll_db=search_db(&db_list_head, ptr_cr->db_name))==NULL){
            PDEBUG_HANDLECLIENTS("new db is not present in memory...Create one \n");

            ll_db=alloc_ll_node();
            db=alloc_db_node();

            get_db(ptr_cr->db_name, &(db->db_rec));

            db->cl_head=NULL;
            db->total_clients=0;

            ll_db->data=(void *)db;
            add(&db_list_head, ll_db);
        }
        else{
            db=(struct db_node *)(ll_db->data);
        }

        if((ll_client=search_client(&client_list_head, sender))==NULL){
            PDEBUG_HANDLECLIENTS("Client is not present on the client list\n");
            exit(0);
        }

        // remove client from client list and add it on db-list
        rem(&client_list_head, ll_client);
        // put client on new db-node
        add(&(db->cl_head), ll_client);
        db->total_clients=db->total_clients+1;

        // send dboard to clients
         send_dboard(db);
    }
/*
    //Add Discussion Board on in memory db list
    if((ll_db=search_db(&db_list_head, ptr_cr->db_name))==NULL){

        ll_db=alloc_ll_node();
        db=alloc_db_node();

        get_db(ptr_cr->db_name, &(db->db_rec));

        db->cl_head=NULL;
        db->total_clients=0;
        //NEED TO FILL FILE FIELD

        ll_db->data=(void *)db;
        add(&db_list_head, ll_db);
    }
    else{
        db=(struct db_node *)(ll_db->data);
    }

    if(!strcmp(ptr_cr->obj, "-1")){ // the client is joining its first dboard
    //Removing from client list and adding on corresponding discussion board// 
        if((ll_client=search_client(&client_list_head, sender))==NULL){
            PDEBUG_HANDLECLIENTS("Client is not present on the client list\n");
            exit(0);
        }
    }
    else{ //remove from old db-list and add to new db-list
        ll_db
    }
     //Remove clients from the client list and put it on the db-client list
    rem(&client_list_head, ll_client);
    add(&(db->cl_head), ll_client);
    db->total_clients=db->total_clients+1;
    send_dboard(db);

//    PDEBUG_HANDLECLIENTS("db-name:-%s\n",db->db_rec.db_name);
//    print_clients(&(db->cl_head));
//    print_clients(&(client_list_head));
*/
    return;
}

void send_dboard(struct db_node *db)
{
    struct obj_record *obj_rec=alloc_obj_record();
    size_t read_bytes=0;
    int i=0;
    int ret;

    struct resp_client response;

    if(!(db->fp=fopen(db->db_rec.db_file,"r+"))){
        printf("(%s,%d) error in opening the dboard file...exiting\n", __func__, __LINE__);
    }
    read_bytes=read_obj(db->fp, i*OBJ_RECORD_SIZE, (char *)obj_rec);

    int x=0;

    while(read_bytes == OBJ_RECORD_SIZE){
        if(obj_rec->status){
            response.packet_type = packet_Response;
            response.obj_id=obj_rec->obj_id;
            response.server_index=obj_rec->server_index;
            response.op='a';

            printf("\n this is x=%d\n", x);

            memcpy(response.db_name, obj_rec->db_name, DB_NAME_SIZE);
            memcpy(response.obj, obj_rec->object, OBJ_SIZE);

//            serve_clients(response);

            ret = SP_multicast(
                                Mbox,
                                AGREED_MESS,
                                (const char *)sender,
                                1,
                                sizeof(struct resp_client),
                                (char *)&response
                    );


            if( ret < 0 )
            {
                SP_error( ret );
                Bye();
            }


        }
        i++;
        read_bytes=read_obj(db->fp, i*OBJ_RECORD_SIZE, (char *)obj_rec);
    }

    return;
}

void serve_clients(struct resp_client response)
{
    /*
     * Get the list of the clients for this disc-board
     * do multigroup multicast
     */

    int ret;
    int i;
    struct ll_node *ll_db;
    struct db_node *db;

    char cl_priv_grp[10][MAX_GROUP_NAME]; /*Decide array size dynamically*/
    
    if((ll_db=search_db(&db_list_head, response.db_name))==NULL){
        PDEBUG_HANDLECLIENTS("no client is not joined to this db\n");
        // No client is joined to this server. So be happy -)
    }
    else{
        PDEBUG_HANDLECLIENTS("There is at least one client joined to this db\n");
        db=(struct db_node *)(ll_db->data);
        struct ll_node *tmp;
        struct client_node *tmp_client;
        tmp=db->cl_head;

        /*
         * Loop over the client list of the this db and add it to array 
         * to do multigroup multicast 
         */
        i=0;
        if(tmp!=NULL){
            do{
                tmp_client=(struct client_node *)(tmp->data);
                PDEBUG_HANDLECLIENTS("%s\n", tmp_client->client_private_group);
                memcpy(cl_priv_grp[i], tmp_client->client_private_group,\
                        MAX_GROUP_NAME);
                tmp=tmp->next;
                i++;
            }while(tmp!=(db->cl_head));
        }

        /*Do multigroup multicast*/

        ret = SP_multigroup_multicast( 
                                        Mbox, 
                                        AGREED_MESS, 
                                        i,     // num_groups, The total groups whom i am sending
                                        (const char (*)[MAX_GROUP_NAME]) cl_priv_grp, 
                                        1, 
                                        sizeof(struct resp_client), 
                                        (char *)&response
                                    );
        if( ret < 0 )
        {
            SP_error( ret );
            Bye();
        }
    }
    return; 
}

void send_view()
{
    int view[NUM_OF_SERVERS];
    int i;
    int ret;

    struct resp_client response;
    response.packet_type=packet_Response;
    response.op='v';
    response.obj_id=-1;
    response.server_index=-1;

    for(i=0; i<NUM_OF_SERVERS; i++){
        if(server_view[i]){
            view[i]=i+1;
        }
        else{
            view[i]=-1;
        }
    }
    memcpy(response.obj, (char *)view, OBJ_SIZE);


    for(i=0; i<NUM_OF_SERVERS; i++){
        printf("%d\t",view[i]);
    }

    // return the message back to client
    ret = SP_multicast(
                        Mbox,
                        AGREED_MESS,
                        (const char *)sender,
                        1,
                        sizeof(struct resp_client),
                        (char *)&response
                      );
    if( ret < 0 )
    {
        SP_error( ret );
        Bye();
    }

//    serve_clients(response); View should not sent to all the connected clients

    return;
}


void remove_client(char *cl_pri_grp)
{
    struct ll_node *ll_client;

    struct db_node *db=NULL;

    struct client_node *cl_node;

    ll_client=find_db_client(&db_list_head, cl_pri_grp, &db);


    if(ll_client != NULL){
        rem(&(db->cl_head), ll_client);
    }
    else{
        ll_client=search_client(&client_list_head, cl_pri_grp);
        rem(&client_list_head, ll_client);
    }

    cl_node=(struct client_node *)(ll_client->data);
   
    SP_leave(Mbox, cl_node->client_server_group);

    PDEBUG_HANDLECLIENTS("The client server_group %s\n", cl_node->client_server_group);
    PDEBUG_HANDLECLIENTS("The client is removed %s\n", cl_node->client_private_group);
    
    return;
}

void print_request(const struct req_client *cr)
{
    printf("------------------------------------------------------------------\n");
    printf("\nREQUEST RECEIVED:\n");
    printf("\tpacket type   : %d\n", cr->packet_type);
    printf("\toperation     : %c\n", cr->op);
    printf("\tobj_id        : %d\n", cr->obj_id);
    printf("\tserver_index  : %d\n", cr->server_index);
    printf("\td-board name  : %s\n", cr->db_name);
    printf("\tobject        : %s\n", cr->obj);
    printf("------------------------------------------------------------------\n");
    
    return;
}
