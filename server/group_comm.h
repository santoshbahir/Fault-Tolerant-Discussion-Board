#ifndef _GROUP_COMM_H
#define _GROUP_COMM_H

#include  "sp.h"
#include "typedefine.h"

#define MAX_VSSETS      10
#define MAX_MEMBERS     100

char     User[80];
char     Spread_name[80];

char     *rs_group;//"SERVERS_REPLICATION_GROUP";
char     server_public_group[20];   //TBD: Nedd to remove hard-coding of 20

int      my_server_id;

char     groups[10][MAX_GROUP_NAME]; //TBD: Need to remove hard-coding of 10
int      To_exit;
char     sender[MAX_GROUP_NAME];
char     Private_group[MAX_GROUP_NAME];

/******************************************************************************
 * This function connect to the spread server. Need to change spread_name and *
 * user_name here when spread is configured locally.                          *
 ******************************************************************************/
void connect_spread();


/******************************************************************************
 * This function joins the group identified by group_name.                    *
 * INPUT    :                                                                 *
 *       group_name - group to be joined                                      *
 ******************************************************************************/
void join_group(const char *group_name);


/******************************************************************************
 * This function creates own public group name.                               *
 ******************************************************************************/
void create_own_group_name();


/******************************************************************************
 * This function reads the messages from the spread and takes the action      *
 * depending upon the type of the message.                                    *
 ******************************************************************************/
void read_message();


/******************************************************************************
 * This function exit the program                                             *
 ******************************************************************************/
void Bye();

#endif
