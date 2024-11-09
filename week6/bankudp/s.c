#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
    int acc_no;
    int pin;
    int balance;
} Account;

// Predefined accounts
Account accounts[5] = {
    {101, 1234, 5000},
    {102, 5678, 8000},
    {103, 9101, 3000},
    {104, 1213, 1500},
    {105, 1415, 6000}
};

void handle_client(int sfd, struct sockaddr_in *client_addr, socklen_t client_len, char *buffer) {
    int acc_no, pin, withdraw;
    char response[100];
    
    sscanf(buffer, "%d %d %d", &acc_no, &pin, &withdraw);

    int found = 0;
    for (int i = 0; i < 5; i++) {
        if (accounts[i].acc_no == acc_no && accounts[i].pin == pin) {
            found = 1;
            if (accounts[i].balance >= withdraw + 1000) {
                accounts[i].balance -= withdraw;
                snprintf(response, sizeof(response), "Withdrawal successful. New balance: %d", accounts[i].balance);
            } else {
                snprintf(response, sizeof(response), "Insufficient balance. Available: %d (Minimum balance required: 1000)", accounts[i].balance);
            }
            break;
        }
    }

    if (!found) {
        strcpy(response, "Invalid Account Number/PIN.");
    }

    // Send response to client
    sendto(sfd, response, strlen(response), 0, (struct sockaddr*)client_addr, client_len);
}

int main() {
    int sfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[100];

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(3001);

    if (bind(sfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sfd);
        exit(1);
    }

    printf("Server listening on port 3001\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        
        // Receive data from client
        recvfrom(sfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
        
        // Process the client's request
        handle_client(sfd, &client_addr, client_len, buffer);
    }

    close(sfd);
    return 0;
}
