#ifndef _FILE_H
#define _FILE_H

#include <stdbool.h>
#include <stdio.h>
#include "typedefine.h"

//char meta_data_file[FILE_NAME_SIZE];    /*Meta data of db*/
char log_file[FILE_NAME_SIZE];          /*log file of server*/
//FILE *fp;                             /*The FILE pointer to metadata file*/
FILE *fp_log;                           /*The FILE pointer to log file*/

/******************************************************************************
 *This funciton read the object in the file from the corresponding db         *
 ******************************************************************************/
size_t read_obj(FILE *fp_db, off_t offset, char *obj_buffer);

/******************************************************************************
 *This funciton write the object in the corresponding db file                 *
 ******************************************************************************/
size_t write_obj(FILE *fp_db, off_t offset, char *obj_buffer);

/******************************************************************************
 * This function appends the 'size' number of characters present at location  *
 * pointed to by buffer in the file fp                                        *
 ******************************************************************************/
size_t append(FILE *fp, size_t size, char *buffer);

/******************************************************************************
 * This function search the object<lts, server_id> in the file pointed        *
 * db_file_name and, if object presents, copy the data in obj_buffer          *
 ******************************************************************************/
off_t search_obj(const char *db_file_name, int lts, int server_id, \
                    struct obj_record *obj_buffer);

/******************************************************************************
 *This funciton search the server metadata file to see if this db is present  *
 *if present then returns true and the corresponding db_md record is present  *
 *in the buffer pointed by db_buffer.                                         *
 ******************************************************************************/
bool search_db_name(FILE *fp_md, const char *db_name, char *db_buffer);

void write_server_info();

off_t search_log(int lts, int server_id,struct update_record *rec_update);

#endif
