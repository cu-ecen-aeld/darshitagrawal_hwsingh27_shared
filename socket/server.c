/*https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 *Modified by: Darshit Agrawal*/

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <signal.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

struct mq_attr attr;
mqd_t mqd;
int sockfd;
bool signal_indication = false;
// Function designed for chat between client and server.

void graceful_exit()
{
    if(sockfd > -1)
    {
        shutdown(sockfd, SHUT_RDWR);
    }
}

static void signal_handler(int signal)
{
    switch(signal)
    {
        case SIGINT:
          printf("\n\rSIGINT Signal Detected.");
          break;
          
        case SIGTERM:
          printf("\n\rSIGTERM Signal Detected.");
          break;
          
        default:
          printf("\n\rSome Signal Detected.");
          break;
     }
    signal_indication = true;
    graceful_exit();
    exit(0);
}

void func(int connfd)
{
    char buff[sizeof(double) + sizeof(double)];
    char toClient[50];
    unsigned int priority;
    double temperature_data, humidity_data;
    // infinite loop for chat
    while(!signal_indication) 
    {
	if(mq_receive(mqd, buff, sizeof(double) + sizeof(double), &priority) == -1)
	{
	    printf("\n\rError in receiving message from the queue. Error: %s", strerror(errno));
	}
	memcpy(&temperature_data, buff, sizeof(double));
	memcpy(&humidity_data, buff + sizeof(double), sizeof(double));
	sprintf(toClient, "Temperature = %0.2lf and Humidity = %0.2lf", temperature_data, humidity_data);
	send(connfd, toClient, strlen(toClient) + 1, 0);
    }
}

// Driver function
int main()
{
    int connfd, len;
    struct sockaddr_in servaddr, cli;
    
    sig_t rs = signal(SIGINT, signal_handler);
    if (rs == SIG_ERR) 
    {
        printf("\n\rError in registering SIG_ERR.");
        graceful_exit();
        exit(1);
    }

    rs = signal(SIGTERM, signal_handler);
    if (rs == SIG_ERR) 
    {
        printf("\n\rError in registering SIGTERM.");
        graceful_exit();
        exit(1);
    }
    
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
	printf("socket creation failed...\n");
	exit(0);
    }
    else
    {
	printf("Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));
    
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
	exit(0);
    }
    else
    {
	printf("Socket successfully binded..\n");
    }
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) 
    {
	printf("Listen failed...\n");
	exit(0);
    }
    else
    {
	printf("Server listening..\n");
	len = sizeof(cli);
    }
    
    mqd = mq_open("/sendmq", O_RDWR);
    if(mqd == -1)
    {
        printf("\n\rError in opening the message queue. Error: %s", strerror(errno));
    }
    // Accept the data packet from client and verification
    while(signal_indication == false)
    {
        connfd = accept(sockfd, (SA*)&cli, (socklen_t*)&len);
        if (connfd < 0) 
        {
	    printf("server accept failed...\n");
	    exit(0);
        }
        else
        {
	    printf("server accept the client...\n");
        }
    // Function for chatting between client and server
        func(connfd);
    }
    // After chatting close the socket
    close(sockfd);
    close(connfd);
    printf("\n\rConnection closed...");
    return 0;
}

