//client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    int cfd;
    struct sockaddr_in server;
    char buffer[100];
    int acc_no, pin, withdraw;

    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(3001);

    if (connect(cfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Connection to server failed");
        close(cfd);
        exit(1);
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);
    printf("Enter PIN: ");
    scanf("%d", &pin);
    printf("Enter Withdrawal Amount: ");
    scanf("%d", &withdraw);

    snprintf(buffer, sizeof(buffer), "%d %d %d", acc_no, pin, withdraw);
    send(cfd, buffer, strlen(buffer), 0);

    memset(buffer, 0, sizeof(buffer));
    recv(cfd, buffer, sizeof(buffer), 0);

    printf("%s\n", buffer);

    close(cfd);
    return 0; 
}