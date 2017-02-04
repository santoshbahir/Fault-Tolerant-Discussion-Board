#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdbool.h>
#include "typedefine.h"

/*
struct req_client{
    int         packet_type;
    char        op;
    int         obj_id;
    int         server_index;
    char        db_name[20];
    char        obj[80];
};

struct request{
    char client [20];
    struct req_client req;
    
    struct request *prev;
    struct request *next;
};
*/
struct request *queueTop;

bool queueEmpty();

void enqueueReq(struct request *r);

struct request *dequeueReq();

void print_queue();
#endif
