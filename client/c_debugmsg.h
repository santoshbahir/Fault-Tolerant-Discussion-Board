#ifndef DEBUGMSG_H
#define DEBUGMSG_H

/*New Line Printing*/
#define PRINT_NEWLINE

#ifdef PRINT_NEWLINE
#define PRINTNL printf("\n")
#else
#define PRINTNL
#endif

/*CLIENT.H*/
#define DEBUG_MSG_CLIENT

#ifdef DEBUG_MSG_CLIENT
#define PDEBUG_CLIENT(fmt, args...) printf("(%s:%d) " fmt, __func__, __LINE__, ## args)
#define PINOUT_CLIENT(fmt, args...) printf("(%s:%d) IO ARGUMENTS: " fmt, __func__, __LINE__, ## args)
#else
#define PDEBUG_CLIENT(fmt, args...)
#define PINOUT_CLIENT(fmt, args...) 
#endif


/*SPREAD_COMM.H*/
#define DEBUG_MSG_SPREAD_COMM

#ifdef DEBUG_MSG_SPREAD_COMM
#define PDEBUG_SPREAD_COMM(fmt, args...) printf("(%s:%d) " fmt, __func__, __LINE__, ## args)
#define PINOUT_SPREAD_COMM(fmt, args...) printf("(%s:%d) IO ARGUMENTS: " fmt, __func__, __LINE__, ## args)
#else
#define PDEBUG_SPREAD_COMM(fmt, args...)
#define PINOUT_SPREAD_COMM(fmt, args...) 
#endif


/*SERVER_COMM.H*/
#define DEBUG_MSG_SERVER_COMM

#ifdef DEBUG_MSG_SERVER_COMM
#define PDEBUG_SERVER_COMM(fmt, args...) printf("(%s:%d) " fmt, __func__, __LINE__, ## args)
#define PINOUT_SERVER_COMM(fmt, args...) printf("(%s:%d) IO ARGUMENTS: " fmt, __func__, __LINE__, ## args)
#else
#define PDEBUG_SERVER_COMM(fmt, args...)
#define PINOUT_SERVER_COMM(fmt, args...) 
#endif


/*LINKED_LIST.H*/
#define DEBUG_MSG_LINKED_LIST

#ifdef DEBUG_MSG_LINKED_LIST
#define PDEBUG_LINKED_LIST(fmt, args...) printf("(%s:%d) " fmt, __func__, __LINE__, ## args)
#define PINOUT_LINKED_LIST(fmt, args...) printf("(%s:%d) IO ARGUMENTS: " fmt, __func__, __LINE__, ## args)
#else
#define PDEBUG_LINKED_LIST(fmt, args...)
#define PINOUT_LINKED_LIST(fmt, args...) 
#endif

#endif    /*DEBUGMSG*/
