#ifndef _TYPES_H
#define _TYPES_H

#include "include/sp.h"

#define SERVER_NAME_LEN 20

/*Operation from client*/
#define OP_CONNECT  'c'
#define OP_JOIN     'j'
#define OP_CREATE   'a'
#define OP_DELETE   'd'
#define OP_EDIT     'e'

#define DB_NAME_SIZE 20
#define OBJ_SIZE     80

#define NUM_OF_SERVERS 5


typedef enum {
    packet_Client,              // The request from client
    packet_Response
}packet_t;

struct req_client{
    packet_t    packet_type;
    char        op;    
    int		obj_id; // similar to lts
    int		server_index;
    char	db_name[DB_NAME_SIZE];
    char        obj[OBJ_SIZE];
    
};

struct resp_client{
    packet_t 	packet_type;
    char	    op;
    int         obj_id;
    int 	    server_index;
    char        db_name[DB_NAME_SIZE];
    char 	    obj[OBJ_SIZE];
};

char        User[80];
char        Spread_name[80];

char        groups[10][MAX_GROUP_NAME];
int         To_exit;
char        Private_group[MAX_GROUP_NAME];
char        client_server_group[MAX_GROUP_NAME+8];
char        server_list[5][SERVER_NAME_LEN];
int	        cs_index;
int	        client_connected;
int	        joined_db;
char        dboard[DB_NAME_SIZE];
mailbox     Mbox;
int         obj_num;        //These objects are displayed

bool viewRequested; //This is special flag for view
bool disconnect;
#endif

