#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <gpiod.h>

#define GPIO0 "/dev/gpiochip0"
#define DATA (5)

struct gpiod_chip *chip;
struct gpiod_line *data_line;

int humidity_int, humidity_dec;

int receive(void)
{
    int rv_data = 0, count;
    for(count = 0; count < 8; count++)
    {
        while(gpiod_line_get_value(data_line) == 0);
        usleep(30);
        if(gpiod_line_get_value(data_line) == 1)
        {
            printf("\n\rData is HIGH");
            rv_data = (rv_data << 1) | (0x01);
        }
        else
        {
            printf("\n\rData is LOW");
            rv_data = rv_data << 1;
        }
        while(gpiod_line_get_value(data_line) == 1);
    }
    return rv_data;
}

int main()
{
    int return_value;
    int test = 7;
    
    chip = gpiod_chip_open(GPIO0);
    if(chip == NULL)
    {
        fprintf(stderr, "Error in opening the GPIO chip 0. Error = %s", strerror(errno));
        return -1;
    }
    
    data_line = gpiod_chip_get_line(chip, DATA);
    if(data_line == NULL)
    {
        fprintf(stderr, "Error in opening the desired GPIO line. Error = %s", strerror(errno));
        gpiod_chip_close(chip);
        return -1;
    }
    
    while(1)
    {
        return_value = gpiod_line_request_output(data_line, "humidity_line", 0);
        if(return_value)
        {
            fprintf(stderr, "Error in setting the line as output. Error = %s", strerror(errno));
            gpiod_chip_close(chip);
            return -1;
        }
    
        return_value = gpiod_line_set_value(data_line, 0);
        if(return_value)
        {
            fprintf(stderr, "Error in setting the given GPIO line LOW. Error = %s", strerror(errno));
            gpiod_chip_close(chip);
            return -1;
        }
        usleep(18000);
        return_value = gpiod_line_set_value(data_line, 1);
        if(return_value)
        {
            fprintf(stderr, "Error in setting the given GPIO line HIGH. Error = %s", strerror(errno));
            gpiod_chip_close(chip);
            return -1;
        }
    	
    	gpiod_line_release(data_line);
    	
        return_value = gpiod_line_request_input(data_line, "humidity_line");
        if(return_value)
        {
            fprintf(stderr, "Error in setting the line as input. Error = %s", strerror(errno));
            gpiod_chip_close(chip);
            return -1;
        }
    
        while(gpiod_line_get_value(data_line) == 1);
        while(gpiod_line_get_value(data_line) == 0);
        while(gpiod_line_get_value(data_line) == 1);
    
        humidity_int = receive();
        humidity_dec = receive();
    
        printf("\n\rTest value = %d", test);
        printf("\n\rRelative humidity = %d.%d", humidity_int, humidity_dec);
        gpiod_line_release(data_line);
        usleep(1000000);
    }
    return 0;
}
