#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define SAI struct sockaddr_in

void chat(int sockfd,SAI* server) {
    printf("\nClient ready....\n");
    char buff[MAX];
    int len = sizeof(*server);
    while (1) {
        bzero(buff, sizeof(buff));
        printf("Msg:");
        fgets(buff, MAX, stdin);

        sendto(sockfd, buff, sizeof(buff), 0, (SA *) server, len);

        bzero(buff, sizeof(buff));
        recvfrom(sockfd, buff, sizeof(buff), 0, (SA *) server, (socklen_t *) &len);
        printf("Server: %s\n", buff);
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main() {
    SAI server;
    int sockfd;
    sockfd = socket(AF_INET,SOCK_DGRAM, 0);
    server.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
    server.sin_port = htons(PORT);

    chat(sockfd, &server);
    close(sockfd);
    return 0;
}
