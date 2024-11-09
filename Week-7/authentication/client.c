#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    char username[50], password[50];

    // Create a socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up server address information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // Change this to the server's port
    server_addr.sin_addr.s_addr = INADDR_ANY; // Change this to the server's IP address

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Input username and password
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0; // Remove newline character from input

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0; // Remove newline character from input

    // Send username and password to the server
    sprintf(buffer, "%s,%s", username, password);
    send(client_fd, buffer, strlen(buffer), 0);

    // Receive the response from the server
    recv(client_fd, buffer, 1024, 0);
    printf("Server: %s\n", buffer);

    // Close the client socket
    close(client_fd);

    return 0;
}

