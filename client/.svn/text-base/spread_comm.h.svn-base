#ifndef _SPREAD_COMM_H
#define _SPREAD_COMM_H

#include <sys/types.h>
#include "object_list.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "sp.h"
#include "types.h"
#include <string.h>
#include <unistd.h>
#include "server_comm.h"

void connect_spread();
void handle_ctrl_quit();
void ctrl_quit(int sig);
void Bye();
void create_client_server_group();
void join_group(const char *group_name);
void Read_message();

#define MAX_MESSLEN     102400
#define MAX_VSSETS      10
#define MAX_MEMBERS     100

#endif
 

