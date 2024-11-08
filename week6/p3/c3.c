#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int client_socket;
    int port = 12345;
    struct sockaddr_in server_addr;
    char buffer[1024];
    FILE *output_file;

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter File name : ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    output_file = fopen("two.txt", "w");
    if (output_file == NULL) {
        perror("Could not open file to write");
        close(client_socket);
        return 1;
    }

    while (1) {
        ssize_t bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);
        if (bytes_received < 0) {
            perror("receive failed");
            break;
        }
        buffer[bytes_received] = '\0';

        if (strcmp(buffer, "file-not-found") == 0) {
            printf("File not found on server.\n");
            break;
        } else if (strcmp(buffer, "file-transfer-complete") == 0) {
            printf("File transfer complete.\n");
            break;
        }
        fwrite(buffer, 1, bytes_received, output_file);
    }

    fclose(output_file);
    close(client_socket);
    return 0;
}

