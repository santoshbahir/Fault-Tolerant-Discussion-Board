#include <stdio.h>
#include <stdlib.h>
#include "mem_alloc.h"
#include "list.h"
#include "typedefine.h"

struct ll_node *alloc_ll_node()
{
    struct ll_node *tmp=NULL;
    if((tmp=malloc(sizeof(struct ll_node)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
} 

struct client_node *alloc_client_node()
{
    struct client_node *tmp=NULL;
    if((tmp=malloc(sizeof(struct client_node)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
} 

struct db_node *alloc_db_node()
{
    struct db_node *tmp=NULL;
    if((tmp=malloc(sizeof(struct db_node)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}

struct server_info *alloc_server_info()
{
    struct server_info *tmp=NULL;
    if((tmp=malloc(sizeof(struct server_info)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}

struct obj_record *alloc_obj_record()
{
    struct obj_record *tmp=NULL;
    if((tmp=malloc(sizeof(struct obj_record)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}

struct db_record *alloc_db_record()
{
    struct db_record *tmp=NULL;
    if((tmp=malloc(sizeof(struct db_record)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}

struct update_record *alloc_update_record()
{
    struct update_record *tmp=NULL;
    if((tmp=malloc(sizeof(struct update_record)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}

struct log_record *alloc_log_record()
{
    struct log_record *tmp=NULL;
    if((tmp=malloc(sizeof(struct log_record)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}


struct request *alloc_request()
{
    struct request *tmp=NULL;
    if((tmp=malloc(sizeof(struct request)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}


struct spread_packet *alloc_spread_packet()
{
    struct spread_packet  *tmp=NULL;
    if((tmp=malloc(sizeof(struct spread_packet)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}

struct req_client *alloc_req_client()
{
    struct req_client *tmp=NULL;
    if((tmp=malloc(sizeof(struct req_client)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}

struct resp_client *alloc_resp_client()
{
    struct resp_client *tmp=NULL;
    if((tmp=malloc(sizeof(struct resp_client)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}

struct msg_update *alloc_msg_update()
{
    struct msg_update *tmp=NULL;
    if((tmp=malloc(sizeof(struct msg_update)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}

struct msg_vector *alloc_msg_vector()
{
    struct msg_vector *tmp=NULL;
    if((tmp=malloc(sizeof(struct msg_vector)))== NULL){
        printf("Malloc failed");
        exit(0);
    }
    else
        return tmp;
}
