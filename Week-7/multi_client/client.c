#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create a socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up server address information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // Server's port
    server_addr.sin_addr.s_addr = INADDR_ANY; // Server's IP

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Send and receive data in a loop
    while (1) {
        printf("CLIENT: ");
        fgets(buffer, sizeof(buffer), stdin); // Read from keyboard
        send(client_fd, buffer, 1024, 0);

        recv(client_fd, buffer, 1024, 0);
        printf("SERVER: %s\n", buffer);
    }

    // Close the client socket
    close(client_fd);
    return 0;
}

