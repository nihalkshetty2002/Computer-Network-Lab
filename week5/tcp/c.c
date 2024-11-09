#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int client_socket;
    int port = 12345;
    int buffer_size = 1024;
    struct sockaddr_in server_addr;
    char buffer[1024];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);  // Using TCP (SOCK_STREAM)

    if (client_socket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost IP

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        return -1;
    }

    while (1) {
        int num1, num2;
        int choice;

        // Asking for user input (operation and numbers)
        printf("Enter two numbers: ");
        scanf("%d %d", &num1, &num2);
        printf("Choose an operation:\n");
        printf("1. Add\n");
        printf("2. Subtract\n");
        printf("3. Multiply\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Sending the numbers and the choice to the server
        sprintf(buffer, "%d %d %d", num1, num2, choice);
        send(client_socket, buffer, strlen(buffer), 0);

        // Receive the result from the server
        recv(client_socket, buffer, buffer_size, 0);
        printf("Server: %s\n", buffer);

        // Breaking out of the loop if the user wants to exit
        if (choice == 4) {
            printf("Exiting chat.\n");
            break;
        }
    }

    close(client_socket);
    return 0;
}
