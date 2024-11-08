#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server_socket;
    int port = 12345;
    int buffer_size = 1024;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Server listening on port %d\n", port);

    while (1) {
        recvfrom(server_socket, buffer, buffer_size, 0, (struct sockaddr*)&client_addr, &addr_len);
       // buffer[strcspn(buffer, "\n")] = 0;  // Remove newline character if present

        // Parsing the numbers and the operation choice
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
                sendto(server_socket, result, strlen(result), 0, (struct sockaddr*)&client_addr, addr_len);
                close(server_socket);
                return 0;
            default:
                sprintf(result, "Invalid choice. Please choose a valid operation.");
                break;
        }

        sendto(server_socket, result, strlen(result), 0, (struct sockaddr*)&client_addr, addr_len);
    }

    close(server_socket);
    return 0;
}
