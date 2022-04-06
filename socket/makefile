CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

all: servertest clienttest
default: servertest clienttest

servertest : server.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o servertest server.o
	
clienttest : client.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o clienttest client.o

server.o : server.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c server.c $(LIBS)

clienttest.o : clienttest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c client.c $(LIBS)
	
clean :
	rm -f servertest clienttest *.o
