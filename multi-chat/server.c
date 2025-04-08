#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int client_sockets[MAX_CLIENTS] = {0}; // Store client sockets
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Mutex for thread safety

void broadcast_message(char *msg, int sender_fd) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && client_sockets[i] != sender_fd) {
            send(client_sockets[i], msg, strlen(msg), 0);
        }
    }
    pthread_mutex_unlock(&lock);
}

void* handle_client(void* arg) {
    int client_fd = *(int*)arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            printf("Client disconnected\n");
            close(client_fd);

            // Remove from client list
            pthread_mutex_lock(&lock);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == client_fd) {
                    client_sockets[i] = 0;
                    break;
                }
            }
            pthread_mutex_unlock(&lock);

            break;
        }

        printf("Client: %s\n", buffer);
        broadcast_message(buffer, client_fd);
    }

    return NULL;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_fd, MAX_CLIENTS);

    printf("Server is listening on port 8080...\n");

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        // Add client to list
        pthread_mutex_lock(&lock);

        int i;
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = client_fd;
                break;
            }
        }
        pthread_mutex_unlock(&lock);

        if (i == MAX_CLIENTS) {
            printf("Max clients reached! Connection rejected.\n");
            close(client_fd);
        } else {
            printf("New client connected.\n");
            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, &client_fd);
            pthread_detach(tid); // Automatically clean up thread after exit
        }
    }

    close(server_fd);
    return 0;
}
