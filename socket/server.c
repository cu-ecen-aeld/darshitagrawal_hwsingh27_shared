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

#define PORT "8080"
#define BACKLOG (5)

int main()
{
    int sockfd, new_fd;
    int yes = 1;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    //char s[INET_ADDRSTRLEN];
    int rv;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }
    
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("Error in creating the socket. Error: ");
            continue;
        }
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("Error in setsockopt. Error: ");
            exit(1);
        }
        if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("Error in bind(). Error:");
            continue;
        }
        
        break;
    }
    
    freeaddrinfo(servinfo);
    
    if(p == NULL)
    {
        fprintf(stderr, "\n\rServer failed to bind.");
        exit(1);
    }
    if(listen(sockfd, BACKLOG) == -1)
    {
        perror("Error in listening for socket. Error: ");
        exit(1);
    }
    
    sin_size = sizeof(their_addr);
    printf("\n\rWaiting for connections...");
    new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);
    if(new_fd == -1)
    {
        perror("Error in accepting the connection. Error: ");
        exit(1);
    }
    else
    {
        printf("\n\rConnection accepted.");
    }
    
    if(send(new_fd, "Hello, World!", 13, 0) == -1)
    {
        perror("Error in sending the data. Error: ");
    }
    else
    {
        printf("\n\rString sent.");
    }
    close(new_fd);
    return 0;
}
    
    
    
