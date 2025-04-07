#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void chat(int conn_fd) {
    while (1) {
        char buffer[100] = {0};
        read(conn_fd, buffer, sizeof buffer);
        printf("Client: %s\n", buffer);

        memset(buffer, 0, sizeof buffer);
        printf("Msg: ");
        scanf(" %[^\n]", buffer);
        write(conn_fd, buffer, strlen(buffer) + 1);

        if (strcmp(buffer, "exit") == 0) {
            close(conn_fd);
            return;
        }
    }
}

int main() {
    struct sockaddr_in server, client;
    int PORT = 7776, len;


    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);



    if (bind(sock_fd, (struct sockaddr *) &server, sizeof server) < 0) {
        perror("BIND failed");
        return 1;
    }

    printf("Server listening on PORT: %d", PORT);


    if (listen(sock_fd, 5) != 0) {
        perror("LISTEN failed");
        return 1;
    }


    len = sizeof client;
    int conn_fd = accept(sock_fd, (struct sockaddr *) &client, (socklen_t *) &len);

    chat(conn_fd);

    close(sock_fd);
}
