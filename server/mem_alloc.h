#ifndef _MEM_ALLOC_H
#define _MEM_ALLOC_H

struct ll_node *alloc_ll_node();
struct client_node *alloc_client_node();
struct db_node *alloc_db_node();
struct server_info *alloc_server_info();
struct obj_record *alloc_obj_record();
struct db_record *alloc_db_record();
struct update_record *alloc_update_record();
struct log_record *alloc_log_record();
struct request *alloc_request();
struct spread_packet *alloc_spread_packet();
struct req_client *alloc_req_client();
struct resp_client *alloc_resp_client();
struct msg_update *alloc_msg_update();
struct msg_update *alloc_msg_update();
struct msg_vector *alloc_msg_vector();

#endif
