#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <wait.h>

#define MAX_CLIENTS 3

void manage_client(int);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int child_sockets[MAX_CLIENTS];
    int i;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 5);
    printf("Server is listening on port 8080...\n");

    for (i = 0; i < MAX_CLIENTS; i++) {
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

        if ((child_sockets[i] = fork()) == 0) {
            close(server_fd);
            manage_client(new_socket);
            exit(0);
        }
    }

    for (i = 0; i < MAX_CLIENTS; i++) {
        wait(NULL);
    }

    close(new_socket);
    return 0;
}

void manage_client(int client_socket) {
    char buffer[1024];
    ssize_t bytes_received;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        printf("CLIENT: %s\n", buffer);
        printf("SERVER: ");
        fgets(buffer, sizeof(buffer), stdin); 
        send(client_socket, buffer, 1024, 0);
    }

    close(client_socket);
}

