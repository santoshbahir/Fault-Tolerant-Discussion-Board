#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>
#include "typedefine.h"

/*
struct ll_node{
    struct ll_node *next;
    struct ll_node *prev;
    void *data;
};
*/

struct ll_node *client_list_head;
struct ll_node *db_list_head;

void init_list();

bool is_empty();

struct ll_node *alloc_ll_node();

void add(struct ll_node **head, struct ll_node *node);

void rem(struct ll_node **head, struct ll_node *node);

struct ll_node *search_client(struct ll_node **head, \
                                 char *cp_group);

void print_clients(struct ll_node **head);

struct ll_node *search_db(struct ll_node **head, const char *db_name);

void print_db(struct ll_node **head);

struct ll_node *find_db_client(struct ll_node **head, char *cl_pri_grp,\
                                struct db_node **db);

#endif
