#ifndef _REPLICATION_H
#define _REPLICATION_H

#include "typedefine.h"

struct server_info s_mdata;

/******************************************************************************
 * This function applies the updats to the database files. This updates are   *
 * applied only when received from the spread. When updates are received      *
 *   - They are written to log file first                                     *
 *   - They are written to database file                                      *
-******************************************************************************/
void init_server(int my_server_id);

void get_db(const char *db_name, struct db_record *db_rec);

void create_db_fname(const char *db_name, char *fname);

void update_replicas(const struct req_client *ptr_cr);

bool apply_update(struct spread_packet client_request);

void send_response(struct update_record *rec_update);

void print_update(struct update_record *rec_update);

void print_object(struct obj_record *obj);

void print_server_info();

#endif
