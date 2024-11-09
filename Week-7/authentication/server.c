#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <wait.h>

#define MAX_CLIENTS 3
#define MAX_LENGTH 1024

void authenticate_client(int client_socket);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int child_sockets[MAX_CLIENTS];
    int i;

    // Create a socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up server address information
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(server_fd, 5);
    printf("Server is listening on port 8080...\n");

    for (i = 0; i < MAX_CLIENTS; i++) {
        // Accept a client connection
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

        // Use fork() to create a child process for each client
        if (fork() == 0) {
            // Child process
            close(server_fd);
            authenticate_client(new_socket);
            exit(0);
        }
    }

    // Wait for all child processes to complete
    for (i = 0; i < MAX_CLIENTS; i++) {
        wait(NULL);
    }

    close(new_socket);
    return 0;
}

void authenticate_client(int client_socket) {
    char buffer[MAX_LENGTH];
    char username[MAX_LENGTH], password[MAX_LENGTH];
    char file_username[MAX_LENGTH], file_password[MAX_LENGTH];
    FILE *file;
    int authenticated = 0;

    // Ask for username and password
    recv(client_socket, buffer, MAX_LENGTH, 0);
    sscanf(buffer, "%[^,],%s", username, password);

    // Open the user file for authentication
    file = fopen("user.txt", "r");
    if (file == NULL) {
        perror("File opening failed");
        close(client_socket);
        return;
    }

    // Read through the file to check for matching credentials
    while (fgets(buffer, MAX_LENGTH, file)) {
        sscanf(buffer, "%[^,],%[^;];", file_username, file_password);

        if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0) {
            authenticated = 1;
            break;
        }
    }

    fclose(file);

    // Send authentication result to the client
    if (authenticated) {
        strcpy(buffer, "Successful login\n");
    } else {
        strcpy(buffer, "Login denied\n");
    }

    send(client_socket, buffer, strlen(buffer), 0);
    close(client_socket);
}

