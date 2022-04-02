CROSS_COMPILE = 
CC = $(CROSS_COMPILE)gcc

CFLAGS = -g -Wall -Werror

default: hello_world

all: hello_world

hello_world: hello_world.o
	$(CC) $< -o $@
	
hello_world.o: hello_world.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean: 
	rm -f hello_world hello_world.o

.PHONY: clean
