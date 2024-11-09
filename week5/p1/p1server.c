#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>

int main(){
	int len,sfd,ak;
	char buf1[50],buf2[50];
    	struct sockaddr_in server, client;
    	
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{	printf("creation error");
		exit(0);
	}
	
	server.sin_family =AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(3001);	
	
	bind(sfd,(struct sockaddr*)&server,sizeof(server));
	len=sizeof(client);
	listen(sfd,5);
	ak=accept(sfd,(struct sockaddr*)&client,&len);
	
	if(ak==-1){	
		printf("error in accepting");
		exit(0);
	}
	
	while (1) {
        memset(buf1, '\0', sizeof(buf1));
        recv(ak, buf1, sizeof(buf1), 0);
        if (strcmp(buf1, "exit\n") == 0) {
            printf("Client disconnected.\n");
            break;
        }
        printf("Client: %s\n", buf1);

        memset(buf2, '\0', sizeof(buf2));
        printf("Server: Enter message - ");
        fgets(buf2, sizeof(buf2), stdin);
        //buf2[strcspn(buf2, "\n")] = '\0';   //remove newline 
        send(ak, buf2,sizeof(buf2), 0);
    }

    close(ak);
    close(sfd);
    return 0;
}

	
