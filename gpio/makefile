CC = gcc 
CFLAGS = -Wall
LIBS = -lrt -lgpiod -pthread
OBJFILES = gpio_read.o
TARGET = gpioreadtest
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LIBS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~
