#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>


void chat(int conn_fd) {
    while (1) {
        char buffer[100];
        printf("Msg: ");
        scanf(" %[^\n]", buffer);
        write(conn_fd, buffer, strlen(buffer));

        memset(buffer, 0, sizeof buffer);
        read(conn_fd, buffer, sizeof buffer);
        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            return;
        }
    }
}

int main() {
    int PORT = 7776;
    struct sockaddr_in server;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(sock_fd, (struct sockaddr *) &server, sizeof server) != 0) {
        perror("CONNECT failed");
        return 1;
    }

    chat(sock_fd);
    close(sock_fd);
}
