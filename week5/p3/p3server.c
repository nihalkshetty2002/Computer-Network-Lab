//server
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

Account accounts[5] = {
    {101, 1234, 5000},
    {102, 5678, 8000},
    {103, 9101, 3000},
    {104, 1213, 1500},
    {105, 1415, 6000}
};

void handle_client(int cfd) {
    int acc_no, pin, withdraw;
    char buffer[100];
    memset(buffer, 0, sizeof(buffer));

    recv(cfd, buffer, sizeof(buffer), 0);
    sscanf(buffer, "%d %d %d", &acc_no, &pin, &withdraw);

    int found = 0;
    for (int i = 0; i < 5; i++) {
        if (accounts[i].acc_no == acc_no && accounts[i].pin == pin) {
            found = 1;
            if (accounts[i].balance >= withdraw + 1000) {
                accounts[i].balance -= withdraw;
                snprintf(buffer, sizeof(buffer), "Withdrawal is successful and new balance: %d", accounts[i].balance);
            } else {
                snprintf(buffer, sizeof(buffer), "Not enough balance.(Available: %d ,Min balance 1000/-)", accounts[i].balance);
            }
            break;
        }
    }

    if (!found) {
        strcpy(buffer, "Invalid Account Number/PIN.");
    }

    send(cfd, buffer, strlen(buffer), 0);
    close(cfd);
}

int main() {
    int sfd, cfd;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(3001);

    if (bind(sfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        close(sfd);
        exit(1);
    }

    listen(sfd, 5);

    while (1) {
        cfd = accept(sfd, (struct sockaddr*)&client, &client_len);
        if (cfd < 0) {
            perror("Accept failed");
            close(sfd);
            exit(1);
        }

        handle_client(cfd);
    }

    close(sfd);
    return 0;
}