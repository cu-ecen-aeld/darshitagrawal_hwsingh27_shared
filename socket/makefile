CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

ifeq ($(LDFLAGS),)
	LDFLAGS= -lrt
endif

all: servertest clienttest
default: servertest clienttest

servertest : server.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o servertest server.o $(LDFLAGS)
	
clienttest : client.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o clienttest client.o $(LDFLAGS)

server.o : server.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c server.c $(LDFLAGS)

clienttest.o : clienttest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c client.c $(LDFLAGS)
	
clean :
	rm -f servertest clienttest *.o
