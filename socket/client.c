/*
 * Reference: https://www.geeksforgeeks.org/socket-programming-cc/
 	       https://beej.us/guide/bgnet/html/#sendrecv
 * Author: Harshwardhan Singh
 */

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

//macros
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[42];
    int rv;
    //int n;
    while(1) 
    {
        while((rv = recv(sockfd, buff, sizeof(buff), 0)) != 41);
        if(rv == -1)
        {
            printf("\n\rError: %s", strerror(errno));
            return;
        }
        printf("\n\r%s", buff);
        printf("\n\r%d bytes were received", rv);
        if ((strncmp(buff, "exit", 4)) == 0) 
        {
            printf("Client Exit...\n");
            break;
        }
        //usleep(500000);
    }
}
   
int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;
    char IP[20];
    
    if(argc != 2)
    {
        printf("\n\rPlease enter correct number of arguments with IP Address of the server.");
        return -1;
    }
    printf("\n\rConnecting to the server.");
    strcpy(IP, argv[1]);
    printf("\n\rContents of IP array = %s", IP);
    // socket creation
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // socket verification
    if (sockfd == -1) 
    {
        printf("\n\rSocket creation failed!!Error: %s", strerror(errno));
        return -1;
    }
    else
    {
        printf("\n\rSocket creation successful!!");
    }
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    //storing address of the server
    servaddr.sin_addr.s_addr = inet_addr(IP); 
    // short, network byte order
    servaddr.sin_port = htons(PORT);
   
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) 
    {
        printf("\n\rconnection with the server failed. Error: %s", strerror(errno));
        close(sockfd);
        return -1;
    }
    else
    {
        printf("\n\rconnected to the server.");
    }
   
    // function for chat
    func(sockfd);
   
    // close the socket
    close(sockfd);
}
