#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *receive_messages(void *socket_fd) {
    int sock_fd = *(int *)socket_fd;
    char buffer[50];
    int bytes_received;

    while ((bytes_received = recv(sock_fd, buffer, 50, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("%s", buffer);
    }

    return NULL;
}

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char message[50];
    pthread_t thread_id;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Could not create socket");
        exit(0);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock_fd);
        exit(0);
    }

    printf("Connected to server.\n");

    pthread_create(&thread_id, NULL, receive_messages, (void *)&sock_fd);

    while (1) {
        fgets(message, 50, stdin);
        send(sock_fd, message, strlen(message), 0);
    }

    close(sock_fd);
    return 0;
}
