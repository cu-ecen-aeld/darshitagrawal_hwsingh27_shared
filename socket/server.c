/*https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 *Modified by: Darshit Agrawal*/

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

struct mq_attr attr;
mqd_t mqd;
// Function designed for chat between client and server.
void func(int connfd)
{
    char time_buffer[20];
    char buff[sizeof(double) + sizeof(double) + sizeof(time_buffer)];
    char toClient[100];
    //int n;
    unsigned int priority;
    double temperature_data, humidity_data;
    // infinite loop for chat
    while(1) 
    {
	//bzero(buff, MAX);
	if(mq_receive(mqd, buff, sizeof(double) + sizeof(double) + 20, &priority) == -1)
	{
	    printf("\n\rError in receiving message from the queue. Error: %s", strerror(errno));
	}
	memcpy(&temperature_data, buff, sizeof(double));
	memcpy(&humidity_data, buff + sizeof(double), sizeof(double));
	memcpy(time_buffer, buff + sizeof(double) + sizeof(double), sizeof(time_buffer));
	//printf("\n\rReceived temperature data is : %0.2lf", temperature_data);
	sprintf(toClient, "Temperature = %0.2lf and Humidity = %0.2lf and Time = %s", temperature_data, humidity_data, time_buffer);
        // read the message from client and copy it in buffer
	/*recv(connfd, buff, sizeof(buff), 0);
        // print buffer which contains the client contents
	printf("From client: %s\t To client : ", buff);
	bzero(buff, MAX);
	n = 0;
	// copy server message in the buffer
	while ((buff[n++] = getchar()) != '\n')
	    ;*/

	// and send that buffer to client
	send(connfd, toClient, strlen(toClient) + 1, 0);

	// if msg contains "Exit" then server exit and chat ended.
	/*if (strncmp("exit", buff, 4) == 0) 
	{
	    printf("Server Exit...\n");
	    break;
	}*/
    }
}

// Driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

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

    // After chatting close the socket
    close(sockfd);
}

