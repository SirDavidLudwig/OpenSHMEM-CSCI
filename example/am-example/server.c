#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <sched.h>

#include <mpi.h>

#define PORT    23500

void * network_sr(void * args)
{
    int sockfd, new_socket;
    struct sockaddr_in server_address;
    int opt = 1;
    int addrlen = sizeof(struct sockaddr_in);
    char buffer[32];
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return NULL;
    }
    
    // optional
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("sockopt failed");
        return NULL;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        return NULL;
    }

    if (listen(sockfd, 10) < 0) {
        perror("listen failed:");
        return NULL;
    }

    for (;;) {
        if ((new_socket = accept(sockfd, (struct sockaddr *) &server_address, 
                                         (socklen_t *) &addrlen)) < 0) {
            perror("accept failed: ");
            return NULL;
        }
        
        read(new_socket, buffer, 32);
        printf("Client said: %s\n", buffer);
        close(new_socket);
    }

    return 0;
}

int main(void)
{
    pthread_t network_thread;

    pthread_create(&network_thread, NULL, network_sr, NULL);
    
    // do something useful?
    printf("main thread: I'm doing something useful!\n");
    pthread_join(network_thread, NULL);

    return 0;
}
