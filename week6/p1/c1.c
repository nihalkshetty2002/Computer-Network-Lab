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
    socklen_t addr_len = sizeof(server_addr);

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    while (1) {
        printf("Client: ");
        fgets(buffer, buffer_size, stdin);
        sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, addr_len);

        if (strcmp(buffer, "end\n") == 0) {
            printf("Chat ended.\n");
            break;
	}

        recvfrom(client_socket, buffer, buffer_size, 0, (struct sockaddr*)&server_addr, &addr_len);
        buffer[strcspn(buffer, "\n")] = 0;  // Remove newline character if present
        printf("Server: %s\n", buffer);
    }

    close(client_socket);
    return 0;
}

