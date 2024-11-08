#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int client_socket;
    int port = 12345;
    struct sockaddr_in server_addr;
    char buffer[1024];

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    printf("Enter IP address: ");
    fgets(buffer, 1024, stdin);
    buffer[strcspn(buffer, "\n")] = 0;  // Remove the newline character

    sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    recvfrom(client_socket, buffer, 1024, 0, NULL, NULL);
    printf("Server Response: %s\n", buffer);
    close(client_socket);
    return 0;
}
