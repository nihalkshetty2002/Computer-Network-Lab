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
	 buffer[strcspn(buffer, "\n")] = 0;  // Remove newline character if present
        printf("Client: %s\n", buffer);

        if (strcmp(buffer, "end") == 0) {
            printf("Client has ended the chat.\n");
            break;
        }

        printf("Server: ");
        fgets(buffer, buffer_size, stdin);
        sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addr_len);
    }

    close(server_socket);
    return 0;
}


