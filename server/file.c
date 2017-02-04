#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include "typedefine.h"
#include "file.h"
#include "replication.h"
#include "debugmsg.h"

size_t append(FILE *fp, size_t size, char *buffer)
{
    off_t   offset;
    size_t  write_bytes;

    PDEBUG_LIST("I will append %s\n",buffer);
    if((offset = fseek(fp, 0, SEEK_END)) < 0)
    {
        PDEBUG_LIST("Inside IF\n");
        PDEBUG_LIST("Error in going to end of the file\n");
    }
    else
    {
        if((write_bytes=fwrite(buffer, sizeof(char), size, fp)) < size)
        {
            PDEBUG_LIST("Error in Appending\n");
            exit(0);
        }
    }

    if((offset = fseek(fp, size, SEEK_END)) < 0)
    {
        PDEBUG_LIST("Inside IF\n");
        PDEBUG_LIST("Error in going to end of the file\n");
    }

    return write_bytes;    
}

bool search_db_name(FILE *fp_md, const char *db_name, char *db_buffer)
{
    off_t   offset;
    size_t  read_bytes;
    struct db_record *tmp=NULL;

    if((offset = fseek(fp_md, 0, SEEK_SET)) < 0)
    {
        PDEBUG_LIST("Inside IF\n");
        PDEBUG_LIST("Error in going to start of the file\n");
    }
    else
    {
        while((read_bytes=fread(db_buffer, sizeof(char), DB_RECORD_SIZE,fp_md))\
                            == DB_RECORD_SIZE)
        {
            PDEBUG_LIST("read = %s\n",((struct db_record *)db_buffer)->db_file);
            tmp=(struct db_record *)db_buffer;
            if(strcmp(tmp->db_name, db_name)==0){
                return true;
            }
        }
    }
    return false;
}

/*
 * Searches the db file for the object <lts, server_id> 
 *  -   IF found returns the offset in the file
 *  -   else returns -1
 */
off_t search_obj(const char *db_file_name, int obj_id, int server_id, \
                    struct obj_record *obj_buffer)
{
    off_t offset=-1;
    FILE *fp_db;
    size_t read_bytes;


    if((fp_db=fopen(db_file_name, "r")) == NULL){
        PDEBUG_LIST("Failed to open db-file\n");
        exit(0);
    }
    else{
        while(true){
            if((read_bytes=fread(obj_buffer, sizeof(char), OBJ_RECORD_SIZE, \
                            fp_db)) < OBJ_RECORD_SIZE){
                offset=-1;
                break;
            }
            else{
                /*IF found*/
                if(    obj_buffer->obj_id       == obj_id
                    && obj_buffer->server_index == server_id 
                    && obj_buffer->status       == true
                  )
                {
                    offset=ftell(fp_db)-OBJ_RECORD_SIZE;
                    break;    
                }
            }
        }
    }
    return offset;
}


size_t read_obj(FILE *fp_db, off_t offset, char *obj_buffer)
{
    size_t  read_bytes=0;

    if((offset = fseek(fp_db, offset, SEEK_SET)) < 0)
    {
        PDEBUG_LIST("Error in going to start of the requested record\n");
    }
    else
    {
        if((read_bytes=fread(obj_buffer, sizeof(char), OBJ_RECORD_SIZE,fp_db))\
                            < OBJ_RECORD_SIZE)
        {
                PDEBUG_LIST("Error in reading a block completely:%d\n",read_bytes);
        }
    }
    return read_bytes;
}


size_t write_obj(FILE *fp_db, off_t offset, char *obj_buffer)
{
    size_t  write_bytes;

    if((offset = fseek(fp_db, offset, SEEK_SET)) < 0)
    {
        PDEBUG_LIST("Error in going to start of the requested record\n");
    }
    else
    {
        if((write_bytes=fwrite(obj_buffer, sizeof(char), OBJ_RECORD_SIZE,\
                                fp_db)) < OBJ_RECORD_SIZE)
        {
            PDEBUG_LIST("Error in writing a block completely\n");
        }
    }
    return write_bytes;
}


void write_server_info()
{
    size_t  write_bytes;
    off_t   offset;

    if((offset = fseek(fp_log, 0, SEEK_SET)) < 0)
    {
        PDEBUG_LIST("Error in going to start of the requested record\n");
    }
    else
    {
        if((write_bytes=fwrite((char *)&s_mdata ,sizeof(char),
            sizeof(struct server_info),fp_log)) != sizeof(struct server_info)){
            printf("Unable to write server info....exiting\n");
            exit(0);
        }
    }
    return;    
}


off_t search_log(int lts, int server_id, struct update_record *rec_update)
{
    off_t offset=-1;
    size_t read_bytes;

    if((offset = fseek(fp_log, sizeof(struct update_record), SEEK_SET)) < 0)
    {   
        PDEBUG_LIST("Error in going to end of the metadata record\n");
        exit(0);
    }   

    while(true){
        if((read_bytes=fread(rec_update, sizeof(char), sizeof(struct update_record), \
                        fp_log)) < sizeof(struct update_record)){
            offset=-1;
            break;
        }
        else{
            /*IF found*/
            if(    rec_update->lts          == lts
                && rec_update->server_id    == server_id
              )
            {
                offset=ftell(fp_log)-sizeof(struct update_record);
                break;
            }
        }
    }
    return offset;
}

