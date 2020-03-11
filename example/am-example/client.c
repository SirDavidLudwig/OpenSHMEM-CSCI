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

#define PORT    23500

int main(void)
{
    int sockfd;
    struct sockaddr_in server_address;
    int addrlen = sizeof(struct sockaddr_in);
    char buffer[32];
    char address[32] = "127.0.0.1";

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(address);
    server_address.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Could not connect to host: ");
        return -1;
    }

    sprintf(buffer, "Hello");
    send(sockfd, buffer, strlen(buffer), 0);
    close(sockfd);

    return 0;
}
