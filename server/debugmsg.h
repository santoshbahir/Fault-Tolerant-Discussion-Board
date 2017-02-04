#ifndef DEBUGMSG_H
#define DEBUGMSG_H

/*New Line Printing*/
#define PRINT_NEWLINE

#ifdef PRINT_NEWLINE
#define PRINTNL printf("\n")
#else
#define PRINTNL
#endif

/*SERVER.H*/
//#define DEBUG_MSG_SERVER

#ifdef DEBUG_MSG_SERVER
#define PDEBUG_SERVER(fmt, args...) printf("(%s:%d) " fmt, __func__, __LINE__, ## args)
#define PINOUT_SERVER(fmt, args...) printf("(%s:%d) IO ARGUMENTS: " fmt, __func__, __LINE__, ## args)
#else
#define PDEBUG_SERVER(fmt, args...)
#define PINOUT_SERVER(fmt, args...) 
#endif


/*GROUP_COMM.H*/
//#define DEBUG_MSG_GROUP_COMM

#ifdef DEBUG_MSG_GROUP_COMM
#define PDEBUG_GROUP_COMM(fmt, args...) printf("(%s:%d) " fmt, __func__, __LINE__, ## args)
#define PINOUT_GROUP_COMM(fmt, args...) printf("(%s:%d) IO ARGUMENTS: " fmt, __func__, __LINE__, ## args)
#else
#define PDEBUG_GROUP_COMM(fmt, args...)
#define PINOUT_GROUP_COMM(fmt, args...) 
#endif


/*MEM_ALLOC.H*/
//#define DEBUG_MSG_MEM_ALLOC

#ifdef DEBUG_MSG_MEM_ALLOC
#define PDEBUG_MEM_ALLOC(fmt, args...) printf("(%s:%d) " fmt, __func__, __LINE__, ## args)
#define PINOUT_MEM_ALLOC(fmt, args...) printf("(%s:%d) IO ARGUMENTS: " fmt, __func__, __LINE__, ## args)
#else
#define PDEBUG_MEM_ALLOC(fmt, args...)
#define PINOUT_MEM_ALLOC(fmt, args...) 
#endif


/*HANDLECLIENTS.H*/
//#define DEBUG_MSG_HANDLECLIENTS

#ifdef DEBUG_MSG_HANDLECLIENTS
#define PDEBUG_HANDLECLIENTS(fmt, args...) printf("(%s:%d) " fmt, __func__, __LINE__, ## args)
#define PINOUT_HANDLECLIENTS(fmt, args...) printf("(%s:%d) IO ARGUMENTS: " fmt, __func__, __LINE__, ## args)
#else
#define PDEBUG_HANDLECLIENTS(fmt, args...)
#define PINOUT_HANDLECLIENTS(fmt, args...) 
#endif

/*LIST.H*/
//#define DEBUG_MSG_LIST

#ifdef DEBUG_MSG_LIST
#define PDEBUG_LIST(fmt, args...) printf("(%s:%d) " fmt, __func__, __LINE__, ## args)
#define PINOUT_LIST(fmt, args...) printf("(%s:%d) IO ARGUMENTS: " fmt, __func__, __LINE__, ## args)
#else
#define PDEBUG_LIST(fmt, args...)
#define PINOUT_LIST(fmt, args...) 
#endif

/*FILE.H*/
//#define DEBUG_MSG_FILE

#ifdef DEBUG_MSG_FILE
#define PDEBUG_FILE(fmt, args...) printf("(%s:%d) " fmt, __func__, __LINE__, ## args)
#define PINOUT_FILE(fmt, args...) printf("(%s:%d) IO ARGUMENTS: " fmt, __func__, __LINE__, ## args)
#else
#define PDEBUG_FILE(fmt, args...)
#define PINOUT_FILE(fmt, args...) 
#endif

/*REPLICATION.H*/
//#define DEBUG_MSG_REPLICATION

#ifdef DEBUG_MSG_REPLICATION
#define PDEBUG_REPLICATION(fmt, args...) printf("(%s:%d) " fmt, __func__, __LINE__, ## args)
#define PINOUT_REPLICATION(fmt, args...) printf("(%s:%d) IO ARGUMENTS: " fmt, __func__, __LINE__, ## args)
#else
#define PDEBUG_REPLICATION(fmt, args...)
#define PINOUT_REPLICATION(fmt, args...) 
#endif

#endif    /*DEBUGMSG*/
