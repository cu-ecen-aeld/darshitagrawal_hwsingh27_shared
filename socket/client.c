#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080
#define MAXDATASIZE 100

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAXDATASIZE];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        printf("\n\rError in creating socket.");
        exit(1);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("10.0.0.55");
    servaddr.sin_port = htons(PORT);
    
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
    {
        perror("Error in connecting with the server. Error: ");
        exit(1);
    }
    else
    {
        printf("\n\rConnected to the server...");
    }
    
    while(read(sockfd, buffer, sizeof(buffer)) == 0);
    printf("\n\rReceived data = %s", buffer);
    return 0;
}
