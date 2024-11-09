#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<strings.h>

void main()
{
	void sendfun(int);
	int sockid;
	struct sockaddr_in serv_addr;
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(6001);
	if((sockid=socket(AF_INET,SOCK_STREAM,0))<0)
		printf("client socket creation error");
	if(connect(sockid,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
		printf("client connection error");
	sendfun(sockid);
}

void sendfun(int s)
{
	char d[1024];
	char d1[1024];
	int n;
	printf("\nENTER THE FILENAME:");
	scanf("%s",d);
	n=send(s,d,sizeof(d),0);
	recv(s,d1,sizeof(d1),0);
	printf("file conetent revecived from server is :\n %s", d1);
}
