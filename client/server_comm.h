#ifndef _SERVER_COMM_H
#define _SERVER_COMM_H

#include "c_debugmsg.h"
#include <stdio.h>
#include "spread_comm.h"
#include "sp.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>
#include "object_list.h"


void display_menu();
void handle_user_input();
void print_view(struct resp_client *resp);
void get_view();
void display_prompt();
void re_init();
/* Global object_no that is assigned for every Append Operation */

int n;

#endif
