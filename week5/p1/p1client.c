#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(){
	int sfd,res;
	char buf1[50],buf2[50];
	
	struct sockaddr_in server;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	
	if(sfd==-1){	
		printf("socket creation error");
		exit(0);
	}
	
	server.sin_family =AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(3001); //Must be >2000
	
	res=connect(sfd,(struct sockaddr*)&server,sizeof(server));
	
	if(res==-1)
	{	printf("connection error");
		exit(0);
	}
	
	while(1){
		memset(buf1,'\0',sizeof(buf1));
		printf("CLIENT :enter message (type exit to stop) -");
		fgets(buf1, sizeof(buf1), stdin);
		
		send(sfd,buf1,sizeof(buf1),0);
		
		if(strcmp(buf1,"exit\n")==0){
			break;
		}
		
		recv(sfd,buf2,sizeof(buf2),0);
		printf("SERVER : %s\n",buf2);
	}
	
	close(sfd);
	return 0;
}
		
		
	

