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

//macros
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[50];
    //int n;
    while(1) 
    {
        read(sockfd, buff, sizeof(buff));
        printf("\n\r%s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) 
        {
            printf("Client Exit...\n");
            break;
        }
        usleep(500000);
    }
}
   
int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
   
    // socket creation
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // socket verification
    if (sockfd == -1) 
    {
        printf("Socket creation failed!!\n");
        exit(0);
    }
    else
    {
        printf("Socket creation successful!!\n");
    }
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    //storing address of the server
    servaddr.sin_addr.s_addr = inet_addr("10.0.0.56"); 
    // short, network byte order
    servaddr.sin_port = htons(PORT);
   
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) 
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
    {
        printf("connected to the server..\n");
    }
   
    // function for chat
    func(sockfd);
   
    // close the socket
    close(sockfd);
}
