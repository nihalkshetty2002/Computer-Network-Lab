#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<strings.h>
#include<unistd.h>
#include<fcntl.h>

void main()
{
	void receive(int);
	int sockid, newsocketid,clilen;
	struct sockaddr_in cli_addr, serv_addr;
	
	if((sockid=socket(AF_INET,SOCK_STREAM,0))<0)
		printf("client socket creation error");
		
	bzero((char *)&serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family=AF_INET;
	
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	serv_addr.sin_port=htons(6005);
	
	if(bind(sockid,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
	printf("binding error");
	
	listen(sockid,5);
	
	clilen=sizeof(cli_addr);
	
	newsocketid=accept(sockid,(struct sockaddr *)&cli_addr,&clilen);
	
	if(newsocketid<0)
	printf("accept error");
	
	
	
	receive(newsocketid);
}

void receive(int s)
{
	char s1[1024];
	int n;
	n=recv(s,s1,sizeof(s1),0);
	printf("file name revecived from client is :\n %s", s1);
	
	int fd=open(s1,O_RDONLY);
	read(fd,s1,sizeof(s1));
	send(s,s1,sizeof(s1),0);
	
}
