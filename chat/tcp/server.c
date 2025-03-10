#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024

int main() {
    int PORT;
    struct sockaddr_in server_addr;
    int server_fd, client_fd;
    int addr_len = sizeof(server_addr);

    printf("Enter PORT: ");
    scanf(" %d", &PORT);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("SOCKET");
        exit(1);
    }

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*) &server_addr, addr_len) < 0) {
        perror("BIND");
        exit(1);
    }

    if (listen(server_fd, 0) < 0) {
        perror("LISTEN");
        exit(1);
    }

    printf("Server started listening on PORT %d\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr*) &server_addr, (socklen_t *) &addr_len);

    if (client_fd < 0) {
        perror("ACCEPT");
        exit(1);
    }

    char buffer[BUFF_SIZE] = {0};
    read(client_fd, buffer, BUFF_SIZE);

    printf("CLIENT %s\n", buffer);

    char *resp = "Hello from server";
    send(client_fd, resp, strlen(resp), 0);

    close(client_fd);
    close(server_fd);
}
