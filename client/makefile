CC=gcc
LD=gcc
CFLAGS=-g -Wall 
CPPFLAGS=-I. -Iinclude
SP_LIBRARY_DIR=./

all: client

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

client:  $(SP_LIBRARY_DIR)/libspread-core.a client.o spread_comm.o server_comm.o object_list.o
	$(LD) -o $@ object_list.o client.o spread_comm.o server_comm.o $(SP_LIBRARY_DIR)/libspread-core.a

clean:
	rm -f *.o client

