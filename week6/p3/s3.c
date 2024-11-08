#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server_socket;
    int port = 12345;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    printf("Server is running. Waiting for file requests...\n");

    while (1) {
        recvfrom(server_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client_addr, &addr_len);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline if present
        printf("Received file request for: %s\n", buffer); // Debug output

        FILE *file = fopen(buffer, "r");
        if (file == NULL) {
            printf("File not found: %s\n", buffer); // Debug output
            strcpy(buffer, "file-not-found");
            sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addr_len);
            continue;
        }

        printf("Sending file: %s\n", buffer);
        while (1) {
            size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
            if (bytesRead > 0) {
                sendto(server_socket, buffer, bytesRead, 0, (struct sockaddr*)&client_addr, addr_len);
            }
            if (bytesRead < sizeof(buffer)) {
                break;
            }
        }
        fclose(file);
        strcpy(buffer, "file-transfer-complete");
        sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addr_len);
    }
    close(server_socket);
    return 0;
}


