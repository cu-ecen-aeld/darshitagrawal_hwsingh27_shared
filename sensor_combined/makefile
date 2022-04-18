CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror -ggdb
endif

ifeq ($(LDFLAGS),)
	LDFLAGS= -lrt
endif

all: sensor_combined
default: sensor_combined

sensor_combined : sensor_combined.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o sensor_combined sensor_combined.o $(LDFLAGS)

sensor_combined.o : sensor_combined.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c sensor_combined.c $(LDFLAGS)

clean :
	rm -f *.o sensor_combined
