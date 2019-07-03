#define _GNU_SOURCE 1 // define 在include 前

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

#define USER_LIMIT 5 /* 最大用户数  */
#define BUFFER_SIZE 64  //  缓冲区大小
#define FD_LIMIT 65535  // fd限制


struct client_data{
    struct sockaddr_in client_address;
    int sockfd;
    char * write_buffer;
    char buffer[BUFFER_SIZE];
};

int setnonblocking(int fd)
{
    int old_opt = fcntl(fd,F_GETFL);
    int new_opt = old_opt|O_NONBLOCK;
    fcntl(fd,F_SETFL,new_opt);
    return old_opt;
}



int main(int argc, char **argv)
{
    //check argc
    if (argc <= 2)
    {
        printf("usage: ./%s ip_address port \n", basename(argv[0]));
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);


    //根据IP、port设置sockaddr_in结构体
    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_family=AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);//主机序转网络序

    int ret ;
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    assert(listenfd>=0);

    int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    ret = bind(listenfd,(struct sockaddr*) &address,sizeof(address));
    assert(ret!=-1);

    printf("%d\n",ret);

    ret = listen(listenfd,5);
    assert(ret!=-1);

    client_data *users = (client_data*) malloc(sizeof(client_data)*USER_LIMIT);

    while(1){
        struct sockaddr_in client_addr;
        socklen_t client_addr_length = sizeof(client_addr);
        int clientfd;
		if ((clientfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_length)) >= 0)
        {
			printf("Connected from %s\n", inet_ntoa(client_addr.sin_addr));
            





        };




		// len = send(clientfd,"hello",5,0);

		// len = recv(clientfd,buffer,MAXLENGTH,0);
		// printf("%s\n",buffer);
		break;
		



    }

    free(users);
    close(listenfd);
    
    return 0;
}