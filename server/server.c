#include <stdio.h>
#include <stdlib.h>
#include "group_comm.h"
#include "typedefine.h"
#include "replication.h"
#include "list.h"

static void usage(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    rs_group="SERVERS_REPLICATION_GROUP";

    usage(argc, argv);
    init_server(my_server_id);
    init_list();

    connect_spread();
    join_group(rs_group);
    create_own_group_name(my_server_id);
    join_group(server_public_group);


    /*Recovery*/    

    for(;;){
        read_message();
    }

    /*Replication*/
    
    return 0;
}

static void usage(int argc, char *argv[])
{
    if(argc<2)
    {
        printf("\nMore Arguments Required");
        printf("\nUsage: server <server_ID>\n");
        exit(0);
    }
    my_server_id=atoi(argv[1]);

    return;
}
