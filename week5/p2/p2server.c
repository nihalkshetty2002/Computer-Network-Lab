// Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

int validate_ip(const char *ip) 
{
    int num, dots = 0;
    char *ptr,ip_copy[17];

    strcpy(ip_copy, ip);
    ptr = strtok(ip_copy, ".");

    while (ptr) {
        if (!isdigit(*ptr)) return 0;
        num = atoi(ptr);
        if (num < 0 || num > 255) return 0;
        ptr = strtok(NULL, ".");
        dots++;
    }
    return (dots == 4);
}

int main() {
    int len, sfd, ak;
    char IP[17], netmask[17], class[10], netaddr[17], broadcast[17];

    struct sockaddr_in server, client;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        perror("Socket creation failed");
        exit(0);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(3002);

    bind(sfd, (struct sockaddr*)&server, sizeof(server));
    listen(sfd, 5);

    len = sizeof(client);
    ak = accept(sfd, (struct sockaddr*)&client, &len);
    if (ak < 0) {
        perror("Accept failed");
        close(sfd);
        exit(1);
    }

    memset(IP, '\0', sizeof(IP));
    recv(ak, IP, sizeof(IP), 0);

    printf("Received IP address from client: %s\n", IP);

    int fstbyte;
    sscanf(IP, "%d", &fstbyte);

    strcpy(netmask, "N/A");

    if (validate_ip(IP)) {
        if (fstbyte >= 0 && fstbyte <= 127) {
            strcpy(class, "A");
            strcpy(netmask, "255.0.0.0");
        } else if (fstbyte >= 128 && fstbyte <= 191) {
            strcpy(class, "B");
            strcpy(netmask, "255.255.0.0");
        } else if (fstbyte >= 192 && fstbyte <= 223) {
            strcpy(class, "C");
            strcpy(netmask, "255.255.255.0");
        } else if (fstbyte >= 224 && fstbyte <= 239) {
            strcpy(class, "D");
        } else if (fstbyte >= 240 && fstbyte <= 255) {
            strcpy(class, "E");
        } 
    }else {
        strcpy(class, "Unknown");
    }
    

    if (strcmp(class, "A") == 0 || strcmp(class, "B") == 0 || strcmp(class, "C") == 0) {
        int ip[4], mask[4], network[4], brdcst[4];
        sscanf(IP, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
        sscanf(netmask, "%d.%d.%d.%d", &mask[0], &mask[1], &mask[2], &mask[3]);

        for (int i = 0; i < 4; i++) {
            network[i] = ip[i] & mask[i];
            brdcst[i] = network[i] | (~mask[i] & 255);
        }

        sprintf(netaddr, "%d.%d.%d.%d", network[0], network[1], network[2], network[3]);
        sprintf(broadcast, "%d.%d.%d.%d", brdcst[0], brdcst[1], brdcst[2], brdcst[3]);
    } else {
        strcpy(netaddr, "N/A");
        strcpy(broadcast, "N/A");
    }

    send(ak, class, sizeof(class), 0);
    send(ak, netmask, sizeof(netmask), 0);
    send(ak, netaddr, sizeof(netaddr), 0);
    send(ak, broadcast, sizeof(broadcast), 0);

    close(ak);
    close(sfd);
    return 0;
}