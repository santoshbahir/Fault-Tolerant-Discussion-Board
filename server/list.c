#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "typedefine.h"
#include "mem_alloc.h"
#include "debugmsg.h"


void init_list()
{
    client_list_head=NULL;
    db_list_head=NULL;
}

bool is_empty(struct ll_node *head)
{
    if(head == NULL)
        return true;
    else
        return false;
}

void add(struct ll_node **head, struct ll_node *node)
{
    if(is_empty(*head)){
        node->next=node;
        node->prev=node;
        *head=node;
    }
    else{
        node->next=*head;
        node->prev=(*head)->prev;
        (*head)->prev->next=node;
        (*head)->prev=node;
    }

    return;
}

void rem(struct ll_node **head, struct ll_node *node)
{
    static int i;
    PDEBUG_LIST("=== %d\n", i++);
    if((*head)==node && (*head)->next==node){
        *head=NULL;
        PDEBUG_LIST("Entered = %d\n", i++);
        return;
    }
    else{
        if((*head)==node){
            *head=node->next;
        }

        node->prev->next=node->next;
        node->next->prev=node->prev;
    }

    return;
}

struct ll_node *search_client(struct ll_node **head, \
                                 char *cp_group)
{
    struct ll_node *tmp;
    struct client_node *tmp_client;
    tmp=*head;

    if(!is_empty(*head)){
        if(tmp!=NULL)
        {
            do{
                tmp_client=(struct client_node *)(tmp->data);
                
                if(strcmp(tmp_client->client_private_group, cp_group)==0){
                    return tmp;
                    break;
                }
                tmp=tmp->next;
            }while(tmp!=(*head));
        }
    }
    return NULL;
}

void print_clients(struct ll_node **head)
{
    struct ll_node *tmp;
    struct client_node *tmp_client;
    tmp=*head;

    if(!is_empty(*head)){
        if(tmp!=NULL)
        {   
            do{ 
                tmp_client=(struct client_node *)(tmp->data);
                printf("\t%s\n", tmp_client->client_private_group); 
                tmp=tmp->next;
            }while(tmp!=(*head));
        }   
    }   
    printf("\n");
}

struct ll_node *search_db(struct ll_node **head, const char *db_name)
{
    struct ll_node *tmp;
    struct db_node *tmp_db;
    tmp=*head;

    if(!is_empty(*head)){
        if(tmp!=NULL)
        {
            do{
                tmp_db=(struct db_node *)(tmp->data);

                if(strcmp(tmp_db->db_rec.db_name, db_name)==0){
                    return tmp;
                    break;
                }
                tmp=tmp->next;
            }while(tmp!=(*head));
        }
    }
    return NULL;    
}


struct ll_node *find_db_client(struct ll_node **head, char *cl_pri_grp, \
                            struct db_node **node_db)
{
    // this set of variable will keep track of track dboard
    struct ll_node *db_ll;
    struct db_node *db_node;
    db_ll=*head;
   
    struct ll_node *cl_ll;

    if(!is_empty(*head)){
            if(db_ll!=NULL){
                do{
                    db_node=(struct db_node *)(db_ll->data);

                    // search for the client inside this dboard
                    cl_ll=search_client(&(db_node->cl_head), cl_pri_grp);
           
                    if(cl_ll!=NULL){ //Client is present on db_ll
                        *node_db=db_node;
                        return cl_ll;
                     }

                    db_ll=db_ll->next;

                }while(db_ll!=(*head));
            }
    }
    return NULL; // Client is not present on any db
}


void print_db(struct ll_node **head)
{
    struct ll_node *tmp;
    struct db_node  *tmp_db;
    tmp=*head;

    if(!is_empty(*head)){
        if(tmp!=NULL)
        {   
            do{ 
                tmp_db=(struct db_node *)(tmp->data);
                PDEBUG_LIST("%s\n", tmp_db->db_rec.db_name);
                tmp=tmp->next;
            }while(tmp!=(*head));
        }   
    }   
    return;
}

/*
struct a{
    int x;
};

struct b{
    int y;
};



int main()
{
    PDEBUG_LIST("started\n");
    struct ll_node *tmp, *tmp1;
    struct ll_node *head=NULL;
    
    int i;
   
    struct a *t1;

    for(i=0; i<10; i++){
        t1=(struct a *)malloc(sizeof(struct a));
//        t1->x = ((i%2)==0) ? i : i*10;
        t1->x = i;
        tmp=alloc_ll_node();
        tmp->data=(void *)t1;

        add(&head, tmp);    
    }
    
    tmp=head;

    for(i=0; i<10; i++){
        if(i < 7){
            tmp1=tmp->next;
            tmp=tmp1; 
            continue;
        }
        t1=(struct a *)(tmp->data);
        tmp1=tmp->next;
        rem(&head, tmp);
        tmp=tmp1; 
    }

    tmp=head;
    if(tmp!=NULL)
    {
        do{
            t1=(struct a *)(tmp->data);
            PDEBUG_LIST("i=%d\n",t1->x);
            tmp=tmp->next;
        }while(tmp!=(head));
    }

    return 0;
}

*/
