#ifndef _OBJECT_LIST_H
#define _OBJECT_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "types.h"

struct db_object
{
	int	    object_no; /*This should be removed*/
	int	    obj_id;	
	int	    server_index;
	char	    object[OBJ_SIZE];
	struct  db_object *prev;
	struct  db_object *next;
};

/*This variable helps while swithing the db*/
char curr_db[DB_NAME_SIZE];

void init_list();
void re_init_list();

bool is_empty();

/*Interface for spread comm module*/
struct db_object *search(int obj_id,int server_index);

void append(struct db_object *obj);
void edit(struct db_object *obj);
void del(struct db_object *obj);
void insert(struct db_object *obj);


/*Interface for user input handler*/

/******************************************************************************
 * This function returns the obj_num th object in the object list             *
 * INPUT    :                                                                 *
 *     obj_num - Object number to be searched                                 *
 * OUTPUT   :                                                                 *
 *             - pointer to the obj_num th object                             *
 ******************************************************************************/
struct db_object *search_obj(int obj_num);

void append_obj(char *obj);

void edit_obj(int obj_num, char *obj);

void delete_obj(int obj_num);

void display_board();


#endif
