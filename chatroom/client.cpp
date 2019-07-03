#define _GNU_SOURCE 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
// #define BUFFER_SIZE 64

// int main( int argc,char ** argv)
// {
//     if(argc<=2)
//     {
//         printf("usage: ./%s ip_address port \n",basename(argv[0]));
//         return 1;
//     }
//     return 0;
// }




#define MAXLENGTH 64  
int main(int argc,char **argv)
{  
	int sockfd ,len;
	struct sockaddr_in server_addr;
	char buffer[MAXLENGTH+1];
	fd_set rfds;
	struct timeval tv;
	int ret,maxfd =-1,flags;

	if(argc!=3)
	{
		printf("use: ./server IP port\n");
		exit(-1);
	}
	
	//ŽŽœšsocket
	if((sockfd = socket (AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket");
		exit(-1);
	}
	
	//³õÊŒ»¯·þÎñÆ÷µØÖ·ºÍ¶Ë¿Ú
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	if(inet_aton(argv[1],(struct in_addr *)&server_addr.sin_addr.s_addr) == 0){  
        perror(argv[1]);  
        exit(errno);  
    }  
	
	if ((flags=fcntl(sockfd, F_GETFL)) < 0)
		return -1;
	if (fcntl(sockfd, F_SETFL, flags|O_NONBLOCK) < 0)
		return -1;
	
	//ÔÚÒì²œÄ£ÊœÏÂ£¬ÈÎºÎÇé¿öÏÂconnectµÄ·µ»ØÖµ¶ŒÊÇ-1£¬ŽËÊ±ÐèÍš¹ýerrnoÅÐ¶ÏÁ¬œÓÊÇ·ñ³É¹Š
	connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(errno != EINPROGRESS)    
	{
        perror("connect");  
        exit(errno); 
    }  
	printf("connected server !\n");
	
	while(1)
	{
		//Çå¿ÕŒ¯ºÏ
		FD_ZERO(&rfds);
		//°Ñ±ê×ŒÊäÈëµÄŸä±ú0ŒÓÈëµœŒ¯ºÏ
		FD_SET(0,&rfds);
		maxfd = 0;
		//œ«µ±Ç°socketŸä±úŒÓÈëŒ¯ºÏ
		FD_SET(sockfd,&rfds);
		if(sockfd>maxfd)
			maxfd = sockfd;
		//ÉèÖÃ×îŽóµÈŽýÊ±Œä
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		//select
		ret = select(maxfd+1,&rfds,NULL,NULL,&tv);
		if(ret ==-1){
			printf("select error\n");
			break;
		}
		else if(ret ==0)
			continue;
		else{
			if(FD_ISSET(sockfd,&rfds)){//socket¿É¶Á
				bzero(buffer,MAXLENGTH+1);
				len = recv(sockfd,buffer,MAXLENGTH,0);
				if(len>0)
				printf("recv from server: %s\n",buffer);
				else if(len<0)
						printf("receive failed!\n");
				else{
					printf("server down\n");
					break;
				}
			}
			if(FD_ISSET(0,&rfds)){//ÓÐÊäÈë
				bzero(buffer,MAXLENGTH+1);
				fgets(buffer,MAXLENGTH,stdin);
				buffer[strlen(buffer)-1]='\0' ;
				
				len = send(sockfd,buffer,strlen(buffer),0);
				if(len<0){
					printf("send error\n");
					break;
					}
				else if(len!=0)
					printf("send msg: %s \n",buffer);
			}

		}
		

	}

	close(sockfd);

    return 0;  
}