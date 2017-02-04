#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "handleclients.h"


bool queueEmpty(){
    if(queueTop == NULL){
        return true;
    }
    else{
        return false;
    }
}

void enqueueReq(struct request *r)
{
    //add before queueTop i.e. it is last element in queue
    
    if(queueEmpty()){
        // add as a first element in queue since queue is empty
        r->next=r;
        r->prev=r;
        queueTop=r;
    
        return;
    }

    // add as a last element

    r->next=queueTop;
    r->prev=queueTop->prev;
    
    queueTop->prev->next=r;
    queueTop->prev=r;

    return;
}

struct request *dequeueReq()
{
    // remove queueTop

    struct request *retReq;

    retReq=queueTop;

    if(queueTop->next==queueTop){
        // only one element was present
        queueTop=NULL;
    }
    else{
        queueTop=queueTop->next;

        if(queueTop->next == retReq)
            queueTop->next=retReq->next;

        retReq->prev->next=queueTop;
        queueTop->prev=retReq->prev;
    }
    
    return retReq;
}

void print_queue()
{
    struct request *tmp;

    if(!queueEmpty()){
        printf("##################################################################\n\n");    
        printf("\nQUEUE FORMED BECAUSE OF SYNC:\n");    

        tmp=queueTop;
        do{
            printf("\tCLIENT   : %s\n", tmp->client);
            print_request(&(tmp->req));
            tmp=tmp->next;
        }while(tmp!=queueTop);

        printf("##################################################################\n\n");    
    }

    return;    
}


/*
int main()
{

    struct request *tmp;
    int i=0;
    

    queueTop=NULL;
    for(i=10; i<10; i++){
        if((tmp=(struct request *)malloc(sizeof(struct request)))==NULL){
            printf("Malloc failed\n");
        }
        else{
            tmp->req.obj_id=i;
            enqueueReq(tmp);
        }
    }

    //enqueue test
    tmp=queueTop;

    if(!queueEmpty()){
        do{
            printf("i=%d\n",tmp->req.obj_id);
            tmp=tmp->next;
        }while(tmp!=queueTop);
    }


    //dequeue test
    
    tmp=queueTop;
    struct request *req;
    int k=0;
   
    while(1){
        if(queueEmpty()){
            printf("No more to dequeue..\n");
            break;
        }
        k++;    
        req=dequeueReq();

        printf("Dequeued request:  i=%d \n", req->req.obj_id);
//        printf("Dequeued request:  queueTop=%d | prev=%d | i=%d | next=%d\n",queueTop->req.obj_id, req->prev->req.obj_id, req->req.obj_id, req->next->req.obj_id);
        

        if(k == 8) break;
    } 
    
    return 0;
}*/










