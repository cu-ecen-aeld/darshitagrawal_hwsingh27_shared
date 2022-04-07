#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <gpiod.h>

#define GPIO0 "/dev/gpiochip0"
#define DATA 5
#define MAXTIMINGS 50
#define MIN_CHECK 16
#define HIGH 1

struct gpiod_chip *chip;
struct gpiod_line *data_line;

int dht_data[5] = {0};
int temp_int = 0, temp_dec = 0;
int checksum = 0;

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
    //printf("\n\r%d", rv_data);
    return rv_data;
}

int main()
{
    int return_value;
    //int test = 7;
    uint8_t i, counter;
    uint8_t index = 0;
    uint8_t laststate = HIGH;
    
    chip = gpiod_chip_open(GPIO0);
    if(chip == NULL)
    {
        fprintf(stderr, "Error in opening the GPIO chip 0. Error = %s", strerror(errno));
        return -1;
    }
    
    while(1)
    {
        data_line = gpiod_chip_get_line(chip, DATA);
    	if(data_line == NULL)
    	{
            fprintf(stderr, "Error in opening the desired GPIO line. Error = %s", strerror(errno));
            gpiod_chip_close(chip);
            return -1;
    	}
    	
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
    	usleep(40);
    	gpiod_line_release(data_line);
    	data_line = gpiod_chip_get_line(chip, DATA);
    	if(data_line == NULL)
    	{
            fprintf(stderr, "Error in opening the desired GPIO line. Error = %s", strerror(errno));
            gpiod_chip_close(chip);
            return -1;
    	}
    	
        return_value = gpiod_line_request_input(data_line, "humidity");
        if(return_value)
        {
            fprintf(stderr, "Error in setting the line as input. Error = %s", strerror(errno));
            gpiod_chip_close(chip);
            return -1;
        }
        
        for(i = 0; i <= MAXTIMINGS; i++)
        {
            counter = 0;
            while(gpiod_line_get_value(data_line) == laststate)
            {
                counter++;
                usleep(1);
                if(counter == 255)
                {
                    break;
                }
            }
            laststate = gpiod_line_get_value(data_line);
            if(counter == 255)
            {
                break;
            }
            if((i >= 4) && (i % 2 == 0))
            {
                dht_data[index/8] <<= 1;
                if(counter > MIN_CHECK)
                {
                    dht_data[index/8] |= 1;
                }
                index++;
             }
         }
                
        printf("\n\rRelative humidity = %d.%d", dht_data[0], dht_data[1]);
        gpiod_line_release(data_line);
        usleep(1000000);
    }
    return 0;
}
