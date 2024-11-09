#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server_socket, client_socket;
    int port = 12345;
    int buffer_size = 1024;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[1024];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);  // Using TCP (SOCK_STREAM)

    if (server_socket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return -1;
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close(server_socket);
        return -1;
    }

    printf("Server listening on port %d\n", port);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);  // Accept connection
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        while (1) {
            // Receive the numbers and operation choice from the client
            recv(client_socket, buffer, buffer_size, 0);
            int num1, num2, choice;
            sscanf(buffer, "%d %d %d", &num1, &num2, &choice);

            char result[1024];
            switch (choice) {
                case 1:  // Add
                    sprintf(result, "Result: %d", num1 + num2);
                    break;
                case 2:  // Subtract
                    sprintf(result, "Result: %d", num1 - num2);
                    break;
                case 3:  // Multiply
                    sprintf(result, "Result: %d", num1 * num2);
                    break;
                case 4:  // Exit
                    sprintf(result, "Server: Exiting.");
                    send(client_socket, result, strlen(result), 0);
                    close(client_socket);
                    close(server_socket);
                    return 0;
                default:
                    sprintf(result, "Invalid choice. Please choose a valid operation.");
                    break;
            }

            send(client_socket, result, strlen(result), 0);  // Send the result back to the client
        }
    }

    close(server_socket);
    return 0;
}
