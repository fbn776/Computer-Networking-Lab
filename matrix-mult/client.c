#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct {
    int n;
    int m;
    int arr[10][10];
} Matrix;

void read_matrix(Matrix *in_mat) {
    printf("Enter n m: ");
    scanf(" %d %d", &in_mat->n, &in_mat->m);
    printf("Enter matrix:\n");
    for (int i = 0; i < in_mat->n; i++) {
        for (int j = 0; j < in_mat->m; j++) {
            scanf(" %d", &in_mat->arr[i][j]);
        }
    }
}

int main() {
    int PORT = 7777;
    struct sockaddr_in server;
    Matrix A, B, C;

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    connect(sock_fd, (struct sockaddr *) &server, sizeof server);

    read_matrix(&A);
    write(sock_fd, &A, sizeof A);

    read_matrix(&B);
    write(sock_fd, &B, sizeof B);

    read(sock_fd, &C, sizeof C);

    printf("From Server:\n");
    for (int i = 0; i < C.n; i++) {
        for (int j = 0; j < C.m; j++) {
            printf("%d ", C.arr[i][j]);
        }
        printf("\n");
    }

    close(sock_fd);
}