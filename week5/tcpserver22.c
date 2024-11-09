#include"lib.c"


void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc,char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr,cli_addr;
	struct hostent *server;
	
	char buffer[256];
	
	if(argc<3)
	{
		fprintf(stderr,"useage %s hostname port\n",argv[0]);
		exit(0);
	}
	
	if(sockfd < 0)
	{
		error("ERROR OPENING SOCKET");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	portno = atoi(argv[1]);
	//sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s= AF_INET;
	serv_addr.sin_port = htons(portno);
	
	if(bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
		error("error on binding");
	
	listen(sockfd,5);
	clilenGT\t=sizeof(cli_addr);

	newsocketfd=accept(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
	
	
	printf("PLEase enter the message");
	bzero(buffer,256);
	fgets(buffer,255,stdin);
	n= write(sockfd, buffer, strlen(buffer));
	
	if(n<0)
	{error("ERROR writing to socket");}
	bzero(buffer,256);
	n= write(newsocketfd,buffer,255);
	
	
	if(n<0)
	{error("ERROR writing to socket");}
	
	close(newsockfd);
	close(sockfd);
	return 0;
}
