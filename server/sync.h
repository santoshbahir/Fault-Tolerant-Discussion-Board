#ifndef _SYNC_H
#define _SYNC_H

#include <stdbool.h>
#include "typedefine.h"

bool syncOn;

int num_memb; // total members in the current group

int **summary;

int vector_rcvd;
int syncEnd_rcvd;


/*
 * This function is called whenever there is memebership message received. It
 * starts the synchronization of the replicated servers who are in the current
 * partition. Until partition is done; No server serves its clients. Till then
 * It buffers the client requests.
 */
void sync();

/*
 * This function processes the received vectors from the the replication servers
 * If the total received vectors are equal to the total members in the group,
 * this will initiate the sending of the updates
 */
void process_vector(struct spread_packet vector_msg);


/*
 * This functions first finds out the what messages I am supposed to send. Once
 * that is determined it actually starts sending the messages.
 */
void send_updates();

void process_syncEnd(struct spread_packet syncEnd);

/*
 * This function calculates who is having the maximum and minimum of the lts.if
 * I have maximum messages of particular servers and my younger servers do not
 * have maximum messages for that servers, I will send those messages
 */
void find_my_job(int *server_id, int *min_lts, int *max_lts);


/*
 * This function finds the m_id who has maximum lts of server 'index'. It also  
 * finds the minimum of lts for the server 'index'
 */
void find_max_min(int index, int *m_id, int *max_lts, int *min_lts);


/*
 * This function is debugging support: it prints the details of vector receiveed
 */
void print_vector(struct msg_vector *vector_msg);

#endif
