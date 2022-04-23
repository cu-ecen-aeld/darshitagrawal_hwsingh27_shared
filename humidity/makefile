CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall
endif

all: humiditytest
default: humiditytest

humiditytest : humidity.o 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o humiditytest humidity.o

humidity.o : humidity.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c humidity.c

clean :
	rm -f humiditytest *.o
