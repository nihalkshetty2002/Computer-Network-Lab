#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server_socket;
    int port = 12345;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024], ip_class, default_mask[16];
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    recvfrom(server_socket, buffer, 1024, 0, (struct sockaddr*)&client_addr, &addr_len);
    buffer[strcspn(buffer, "\n")] = 0;  // Remove newline character

    unsigned char first_octet = buffer[0];
    sscanf(buffer, "%hhu", &first_octet);

    if (first_octet >= 1 && first_octet <= 126) {
        ip_class = 'A';
        strcpy(default_mask, "255.0.0.0");
    } else if (first_octet >= 128 && first_octet <= 191) {
        ip_class = 'B';
        strcpy(default_mask, "255.255.0.0");
    } else if (first_octet >= 192 && first_octet <= 223) {
        ip_class = 'C';
        strcpy(default_mask, "255.255.255.0");
    } else if (first_octet >= 224 && first_octet <= 239) {
        ip_class = 'D';
        strcpy(default_mask, "N/A");
    } else {
        ip_class = 'E';
        strcpy(default_mask, "N/A");
    }

    printf("Received IP: %s\n", buffer);
    printf("Class: %c, Default Mask: %s\n", ip_class, default_mask);

    sprintf(buffer, "Class: %c, Default Mask: %s", ip_class, default_mask);
    sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addr_len);
    close(server_socket);
    return 0;
}

