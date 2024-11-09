#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    int cfd;
    struct sockaddr_in server_addr;
    char buffer[100];
    socklen_t server_len = sizeof(server_addr);
    int acc_no, pin, withdraw;

    cfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (cfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(3001);

    // Get account details and withdrawal amount from user
    printf("Enter Account Number: ");
    scanf("%d", &acc_no);
    printf("Enter PIN: ");
    scanf("%d", &pin);
    printf("Enter Withdrawal Amount: ");
    scanf("%d", &withdraw);

    snprintf(buffer, sizeof(buffer), "%d %d %d", acc_no, pin, withdraw);
    
    // Send data to server
    sendto(cfd, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, server_len);

    // Receive response from server
    memset(buffer, 0, sizeof(buffer));
    recvfrom(cfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &server_len);

    // Print server's response
    printf("%s\n", buffer);

    close(cfd);
    return 0;
}
