#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <pigpio.h>

/*
# dhtxx2.c
# 2019-3-3
# Public Domain

gcc -Wall -pthread -o dhtxx2 dhtxx2.c -lpigpio -lrt

sudo ./dhtxx2       # Usage
sudo ./dhtxx2 port  # get 1-wire data from port
*/

uint32_t tick[85];
int  tick_index=0;

void aFunction(int gpio, int level, uint32_t tick_)
{
   tick[tick_index]=tick_;
   tick_index += 1;
}

int main(int argc, char *argv[])
{
   int port;
   int bits[40];
   int humid_1, humid_2, temp_1, temp_2, check_sum;
   double humid, temp;

   if (argc != 2)
   {
       printf("Usage: sudo ./dhtxx port\n");
       printf("port: BCM GPIO port number\n");
       printf("Example: sudo ./dhtxx 24\n");

       return 0;
   }
   else
   {
      port = atoi(argv[1]);
   }

   if (gpioInitialise() < 0) return -1;

   gpioSetMode(port, PI_OUTPUT); //port for output
   gpioWrite(port, 0); // set low
   gpioSleep(PI_TIME_RELATIVE, 0, 18000); // > 18ms
   gpioWrite(port, 1);
   gpioSetMode(port, PI_INPUT); // port for input

   // call aFunction whenever GPIO 24 changes state
   gpioSetAlertFunc(port,aFunction);
   gpioSleep(PI_TIME_RELATIVE, 0, 5000); // sleep for 5000us

   gpioTerminate();


   // raw tick
   // printf("tick amount =%d\n", tick_index);

   for(int i=0; i<tick_index-1;i++) tick[i]=tick[i+1]-tick[i]; // convert into intervals
   //for(int i=0;i<tick_index-1;i++) printf("%d ",tick[i]);
   //printf("\n");

   for(int i=0;i<40;i++) bits[i] = ((tick[i*2+3]>tick[i*2+4])?0:1); // convert into bits
   //for(int i=0;i<40;i++) printf("%d ",bits[i]);
   //printf("\n");

   humid_1=0;
   for(int i=0;i<8;i++){
       humid_1 <<= 1;
       humid_1 += bits[i];
   }

   humid_2=0;
   for(int i=0;i<8;i++){
       humid_2 <<= 1;
       humid_2 += bits[i+8];
   }
   temp_1=0;
   for(int i=0;i<8;i++){
       temp_1 <<= 1;
       temp_1 += bits[i+16];
   }

   temp_2=0;
   for(int i=0;i<8;i++){
       temp_2 <<= 1;
       temp_2 += bits[i+24];
   }

   check_sum=0;
   for(int i=0;i<8;i++){
       check_sum <<= 1;
       check_sum += bits[i+32];
   }
//   printf("%d %d %d %d %d\n",humid_1,humid_2,temp_1,temp_2,check_sum);
   if(((humid_1+humid_2+temp_1+temp_2) & 0xff) == check_sum){
       if((humid_2==0) && (temp_2 == 0)){
           //DHT11
           humid=humid_1;
           temp=temp_1;
       }else{
           //DHT22
           humid=(humid_1*256+humid_2)/10.0;
           if((temp_1 & 0x80)==0){
               temp=(temp_1*256+temp_2)/10.0; //temp>=0
           }else{
               temp=-((temp_1 & 0x7F)*256+temp_2)/10; //temp<0
           };
       };
       printf("Humidity = %3.1f %%RH, Temperature = %-2.1f degree Celsus\n",humid,temp);
       return 0;
   }else{
       printf("Wrong check sum!\n");
       return -1;
   }
}
