CC=gcc
LD=gcc
CFLAGS=-g -Wall 
CPPFLAGS=-I. -Iinclude
SP_LIBRARY_DIR=./

all: server 

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

server:  $(SP_LIBRARY_DIR)/libspread-core.a server.o group_comm.o handleclients.o file.o list.o mem_alloc.o replication.o sync.o queue.o
	$(LD) -o $@ server.o group_comm.o handleclients.o file.o list.o mem_alloc.o replication.o sync.o queue.o $(SP_LIBRARY_DIR)/libspread-core.a

clean:
	rm -f *.o server *.db *.db_data *.log

