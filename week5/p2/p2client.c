// Client
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main() {
    int sfd, res;
    char IP[17], netmask[17], class[10], netaddr[17], broadcast[17];
    struct sockaddr_in server;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        printf("Socket creation error\n");
        exit(0);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(3002);

    res = connect(sfd, (struct sockaddr*)&server, sizeof(server));
    if (res < 0) {
        printf("Connection error\n");
        exit(0);
    }

    printf("Client: Enter the IP address - ");
    scanf("%s", IP);

    send(sfd, IP, sizeof(IP), 0);

    recv(sfd, class, sizeof(class), 0);
    recv(sfd, netmask, sizeof(netmask), 0);
    recv(sfd, netaddr, sizeof(netaddr), 0);
    recv(sfd, broadcast, sizeof(broadcast), 0);
    
    if(strcmp(class,"Unknown")==0)
        printf("Invalid IP\n");
    else
        printf("Class: %s\nMask: %s\nNetwork Address: %s\nBroadcast Address: %s\n", class, netmask, netaddr, broadcast);
    close(sfd);
    return 0;
}