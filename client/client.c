#include "spread_comm.h"
#include "server_comm.h"
#include "types.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "object_list.h"
#include "c_debugmsg.h"
#define _GNU_SOURCE


void init_server_names();
int main()
{
    memcpy(curr_db, "-1",DB_NAME_SIZE);
    obj_num=0;
    disconnect=false;

	handle_ctrl_quit();
	connect_spread();
    init_server_names();
	init_list();	

    system("clear");
	display_menu();
	fflush(stdout);

    E_init();
    E_attach_fd( 0, READ_FD, handle_user_input, 0, NULL, LOW_PRIORITY );
    E_attach_fd( Mbox, READ_FD, Read_message, 0, NULL, HIGH_PRIORITY );
    E_handle_events();

    return 0;
}

void init_server_names()
{
    int i;

    for(i=0; i<5; i++)
    {
        sprintf(server_list[i],"%d_",i+1);
        strcat(server_list[i], "db_server");
    }
    return;
}

