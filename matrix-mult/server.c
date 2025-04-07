#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct {
    int n;
    int m;
    int arr[10][10];
} Matrix;

int main() {
    struct sockaddr_in server, client;
    int PORT = 7777, len = sizeof client;

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);
    bind(sock_fd, (struct sockaddr *) &server, sizeof server);
    printf("Server listening on PORT: %d\n", PORT);
    listen(sock_fd, 5);
    int conn_fd = accept(sock_fd, (struct sockaddr *) &client, (socklen_t *) &len);

    Matrix A, B, C;

    read(conn_fd, &A, sizeof A);
    printf("Got matrix A\n");
    read(conn_fd, &B, sizeof B);
    printf("Got matrix B\n");

    C.n = A.n;
    C.m = B.m;
    for (int i = 0; i < A.m; i++) {
        for (int j = 0; j < B.n; j++) {
            C.arr[i][j] = 0;
            for (int k = 0; k < A.n; k++) {
                C.arr[i][j] += A.arr[i][k] * B.arr[k][j];
            }
        }
    }

    write(conn_fd, &C, sizeof C);
    printf("Matrix C:\n");
    for (int i = 0; i < C.n; i++) {
        for (int j = 0; j < C.m; j++) {
            printf("%d ", C.arr[i][j]);
        }
        printf("\n");
    }

    close(sock_fd);
}
