CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

ifeq ($(LIBS),)
	LIBS= -lgpiod -pthread
endif

all: gpioreadtest gpiowritetest
default: gpioreadtest gpiowritetest

gpioreadtest : gpio_read.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o gpioreadtest gpio_read.o $(LIBS)

gpiowritetest : gpio_write.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o gpiowritetest gpio_write.o $(LIBS)

gpio_read.o : gpio_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c gpio_read.c $(LIBS)

gpio_write.o : gpio_write.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c gpio_write.c $(LIBS)

clean :
	rm -f gpioreadtest gpiowritetest *.o
