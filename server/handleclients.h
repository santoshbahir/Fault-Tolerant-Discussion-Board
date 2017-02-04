#ifndef _HANDLECLIENTS_H
#define _HANDLECLIENTS_H

#include "typedefine.h"
#include "queue.h"

struct request *queueTop;

/******************************************************************************
 * This function handles the request generated at clients connected to this   *
 * server and takes the corresponding actions on keep the replicated server in*
 * sync                                                                       *
 ******************************************************************************/
void handle_client_req(struct spread_packet client_request);


/******************************************************************************
 * When client joins the server, this function puts it to clients list.       *
 ******************************************************************************/
void put_on_client_list(const struct req_client *ptr_client_req);


/******************************************************************************
 * When client joins a db; this function puts the client on the list of       *
 * clients who joined this discussion board and remove from the generic list  *
 * of clients. Also, it reads a file for this discussion board and sends the  *
 * objects to connected clients.                                              *
 ******************************************************************************/
void join_db(const struct req_client *ptr_cr);


void remove_client(char *cl_pri_grp);

/******************************************************************************
 * This is debugging support. It prints the request received from clients.    *
 ******************************************************************************/
void print_request(const struct req_client *cr);


/******************************************************************************
 * When the server receives the updates from other server. The replications   *
 * module generate the response and calls this function. This function sees   *
 * if the update is corresponding to the d-board currenty my clients has      *
 * joined.                                                                    *
 ******************************************************************************/
void serve_clients(struct resp_client response);


void send_view();

#endif
