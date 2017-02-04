#ifndef _TYPEDEFINE_H
#define _TYPEDEFINE_H

#include <stdbool.h>
#include "sp.h"
//#include "list.h"

/*Operation from client*/
#define OP_CONNECT  'c'
#define OP_JOIN     'j'
#define OP_CREATE   'a'
#define OP_DELETE   'd'
#define OP_EDIT     'e'

/*Entity type to be created*/
#define DB          'B'
#define OBJ         'O'
#define INVALID     'I'

#define NUM_OF_SERVERS      5
#define CS_GROUP_NAME_SIZE  MAX_GROUP_NAME+8

#define MAX_MESSLEN     102400

#define DB_NAME_SIZE    20
#define FILE_NAME_SIZE  30
#define OBJ_SIZE        80

#define DB_RECORD_SIZE       sizeof(struct db_record)
#define OBJ_RECORD_SIZE      sizeof(struct obj_record)
 
/*Casting of pointers*/
#define CASTTO_REQ_UPDATE(a)    (struct msg_update *)a
#define CASTTO_REQ_CLIENT(a)    (struct req_client *)a


typedef enum {
    packet_Client,              // The request from client
    packet_Response,
    packet_Update,              // The update from the server
    packet_Vector,              // Vector from other servers
    packet_SyncStart,           // start of sync
    packet_SyncUpdate,          // start of sync
    packet_SyncEnd              // end of sync
}packet_t;

struct ll_node{
    struct ll_node *next;
    struct ll_node *prev;
    void *data;
};


/*Message Identifier*/
/*struct message_ID{
    int     server_id;
    int     server_msg_seq;
};*/


/*obj_id == lts ID*/
struct obj_record{
    int     obj_id;    // (obj_id + server_index)Key to seach in database :-)
    int     server_index;
    int     obj_ver; 
    bool    status;
    char    db_name[DB_NAME_SIZE];  // Foreign Key in db table/file :-)
    char    object[OBJ_SIZE];
};

struct db_record{
    int     db_id;
    char    db_name[DB_NAME_SIZE];  // key to search in database :-)
    char    db_file[FILE_NAME_SIZE]; 
    int     num_of_obj; // total objects created on this server till now
};


struct update_record{
    int     lts;    /*Lamport Timestamp*/
    int     server_id;
    int     update_num;
    char    op;
    char    entity_type; /*DB: create db    OBJ: create object*/
    int     obj_id;
    int     server_index;
    int     obj_ver;
    char    db_name[DB_NAME_SIZE];
    char    obj[OBJ_SIZE];
};

/*##### Do Not use this structure ######*/
struct log_record{
    int                     update_id;
    struct update_record    update;
};


struct server_info{
    int     lts;
    int     update_seq;
    int     server_lts[NUM_OF_SERVERS];
    int     server_updates[NUM_OF_SERVERS];
    char    fillbytes[sizeof(struct update_record)-\
                     ((1 + 1 + NUM_OF_SERVERS + NUM_OF_SERVERS) * sizeof(int))];
};


/*Message Format START*/
struct spread_packet{
    packet_t    packet_type;
    char        payload[MAX_MESSLEN-sizeof(packet_t)];
};

/*Symantic of the req_client changes with the operation type*/
struct req_client{
    packet_t    packet_type;
    char        op;
    int         obj_id;
    int         server_index;
    char        db_name[DB_NAME_SIZE];
    char        obj[OBJ_SIZE]; 
};


struct request{
    char client[MAX_GROUP_NAME];
    struct req_client req;

    struct request *prev;
    struct request *next;
};


struct resp_client{
    packet_t 	packet_type;
    char        op;
    int         obj_id;
    int 	    server_index;
    char        db_name[DB_NAME_SIZE];
    char 	    obj[OBJ_SIZE];
};

struct msg_update{
    packet_t                packet_type;
    struct update_record    update;
};

struct msg_vector{
    int packet_type;
    int server_index;
    int server_vector[NUM_OF_SERVERS];
};
/*Message Format END*/

/*Server in-Memory Structure START*/
struct db_node{
    FILE *fp;
    int    total_clients;
    struct db_record db_rec;
    struct ll_node *cl_head;    /*Client list*/
};


struct client_node{
    char client_server_group[CS_GROUP_NAME_SIZE];   // this is the bi-member group
    char client_private_group[MAX_GROUP_NAME];      // this is client's private group
};

/*Server in-Memory Structure END*/

mailbox Mbox;
bool server_view[NUM_OF_SERVERS];
//int lts;

#endif
