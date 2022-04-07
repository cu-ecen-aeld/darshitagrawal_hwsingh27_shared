#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    char buffer[SIZE];
    while(1) {
        bzero(buffer, sizeof(buffer));
        while(read(sockfd, buffer, sizeof(buffer))==0);  
        printf("From Server : %s\n\r", buffer);
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
   
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else

    bzero(&servaddr, sizeof(servaddr));
  
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("10.0.0.56");
    servaddr.sin_port = htons(PORT);
   
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)      
    { 
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
   
    func(sockfd);     
   
    close(sockfd);    
}
