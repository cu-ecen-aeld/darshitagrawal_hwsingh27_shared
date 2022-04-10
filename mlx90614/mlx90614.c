/* References:
 * https://github.com/torvalds/linux/blob/master/include/uapi/linux/i2c-dev.h
 * https://github.com/torvalds/linux/blob/master/include/linux/i2c.h
 * https://olegkutkov.me/2017/08/10/mlx90614-raspberry/
 *
 * Modified by: Harshwardhan Singh
 */
 
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <unistd.h>
#include <mqueue.h>

/* Macros */
#define MLX90614_TA 			(0x06) //RAM register
#define MLX90614_TOBJ1 		(0x07) //RAM register
#define MLX90614_TOBJ2 		(0x08) //RAM register

#define SLAVE_ADDRESS			(0x5A) //address of the MLX90614 temperature sensor
#define I2C_DEV_PATH 			("/dev/i2c-1")

#define SLEEP_DURATION 		(1000000) // for giving the delay of 1 second

/* Just in case if these were not defined */
#ifndef I2C_SMBUS_READ 
#define I2C_SMBUS_READ 1 
#endif 
#ifndef I2C_SMBUS_WRITE 
#define I2C_SMBUS_WRITE 0 
#endif

struct mq_attr attr;

typedef union i2c_smbus_data i2c_data;

int main()
{
    mqd_t mqd;
    char sensor_buffer[sizeof(double)];
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(double);
    // open i2c bus
    int fdev = open(I2C_DEV_PATH, O_RDWR); 
    if (fdev < 0) 
    {
        fprintf(stderr, "Failed to open I2C interface %s Error: %s\n", I2C_DEV_PATH, strerror(errno));
        return -1;
    }
    
    //storing the address in slave_addr variable
    unsigned char slave_addr = SLAVE_ADDRESS; 
    
    //selectecting the I2C slave device
    if (ioctl(fdev, I2C_SLAVE, slave_addr) < 0) 
    {
        fprintf(stderr, "Failed to select I2C slave device! Error: %s\n", strerror(errno));
        return -1;
    }

    // enable checksums control
    if (ioctl(fdev, I2C_PEC, 1) < 0) 
    {
        fprintf(stderr, "Failed to enable SMBus packet error checking, error: %s\n", strerror(errno));
        return -1;
    }

    // trying to read something from the device using SMBus READ request
    i2c_data data;
    char command = MLX90614_TOBJ1; //setting the command as 0x07
    
    // build request structure
    struct i2c_smbus_ioctl_data sdat = 
    {
        .read_write = I2C_SMBUS_READ,
        .command = command,
        .size = I2C_SMBUS_WORD_DATA,
        .data = &data
    };

    mqd = mq_open("/sendmq", O_CREAT | O_RDWR, S_IRWXU, &attr);
    if(mqd == (mqd_t)-1)
    {
        printf("\n\rError in creating a message queue. Error: %s", strerror(errno));
    }
    while(1)
    {
	// do actual request
	if (ioctl(fdev, I2C_SMBUS, &sdat) < 0) 
	{
       	fprintf(stderr, "Failed to perform I2C_SMBUS transaction, error: %s\n", strerror(errno));
        	return -1;
    	}
	
	// fetching the temperature data from the sensor
	double temperature = (double) data.word; 
	
	// converting the temperature in Celsius using the formula from datasheet
    	temperature = (temperature * 0.02) - 0.01;
    	temperature = temperature - 273.15;

        //logging the temperature
    	printf("Temperature of the busbar = %04.2f\n", temperature);

    	usleep(SLEEP_DURATION); //delay of 1 second
    	memcpy(sensor_buffer, &temperature, sizeof(double));
    	if(mq_send(mqd, sensor_buffer, sizeof(double), 1) == -1)
    	{
    	    printf("\n\rError in sending data via message queue. Error: %s", strerror(errno));
    	}	    
    }
    return 0;
}

