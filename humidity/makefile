CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall
endif

ifeq ($(LDFLAGS),)
	LDFLAGS= -lrt -lpthread -lpigpio
endif

all: humiditytest
default: humiditytest

humiditytest : humidity.o 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o humiditytest humidity.o $(LDFLAGS)

humidity.o : humidity.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c humidity.c $(LDFLAGS)

clean :
	rm -f humiditytest *.o
