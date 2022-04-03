CC = gcc 
CFLAGS = -Wall
LDFLAGS = -lrt -lgpiod -pthread
OBJFILES = gpio_read.o
TARGET = gpioreadtest
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~
