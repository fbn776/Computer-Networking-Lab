#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void chat(int conn_fd) {
    char buffer[100];

    while (1) {
        memset(buffer, 0, sizeof buffer);
        read(conn_fd, buffer, sizeof buffer);
        printf("Client: %s\n", buffer);

        memset(buffer, 0, sizeof buffer);
        printf("Msg: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        write(conn_fd, buffer, strlen(buffer) + 1);

        if (strcmp(buffer, "exit") == 0) {
            close(conn_fd);
            return;
        }
    }
}

int main() {
    struct sockaddr_in server, client;
    int PORT = 7777, len;

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    bind(sock_fd, (struct sockaddr *) &server, sizeof server);

    printf("Server listening on PORT: %d\n", PORT);

    listen(sock_fd, 5);

    len = sizeof client;
    int conn_fd = accept(sock_fd, (struct sockaddr *) &client, (socklen_t *) &len);

    chat(conn_fd);
    close(sock_fd);
}
