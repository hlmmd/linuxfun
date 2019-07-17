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
#include <vector>
#include <unordered_map>
#include <sys/epoll.h>
#include <pthread.h>

#define USER_LIMIT 1000000 /* 最大用户数  */
#define BUFFER_SIZE 64     //  缓冲区大小
//#define FD_LIMIT 65535   // fd限制

using namespace std;

int connected = 0;

struct client_data
{
    struct sockaddr_in client_address;
    char *write_buffer;
    char buffer[BUFFER_SIZE];
};

int setnonblocking(int fd)
{
    int old_opt = fcntl(fd, F_GETFL);
    int new_opt = old_opt | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_opt);
    return old_opt;
}

void addfd(int epollfd, int fd, bool enable_et)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if (enable_et)
    {
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
    connected++;
}

void et(epoll_event *events, int number, int epollfd, int listenfd)
{
    char buf[BUFFER_SIZE];
    for (int i = 0; i < number; i++)
    {
        int sockfd = events[i].data.fd;
        if (sockfd == listenfd)
        {
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof(client_address);

            while (1)
            {
                if (connected >= USER_LIMIT)
                    continue;
                int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
                if (connfd < 0 && (errno == EAGAIN || errno == ECONNABORTED || errno == EPROTO || errno == EINTR))
                    break;
                addfd(epollfd, connfd, true);
                printf("current user:%d\n", connected);
            }
        }
        else if (events[i].events & EPOLLIN)
        {
            while (1)
            {
                memset(buf, '\0', BUFFER_SIZE);
                int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
                if (ret < 0)
                {
                    if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
                    {
                        printf("read later\n");
                        break;
                    }
                    close(sockfd);
                    break;
                }
                else if (ret == 0)
                {
                    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, events);
                    close(sockfd);
                    connected--;
                    printf("current user:%d\n", connected);
                    break;
                }
                else
                {
                    printf("get %d bytes of content: %s\n", ret, buf);
                }
            }
        }
        else
        {
            printf("something else happened \n");
        }
    }
}

int main(int argc, char **argv)
{
    //check argc
    if (argc <= 2)
    {
        printf("usage: ./%s ip_address port \n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    //根据IP、port设置sockaddr_in结构体
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port); //主机序转网络序

    int ret;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    setnonblocking(listenfd);

    ret = listen(listenfd, 10);
    assert(ret != -1);

    epoll_event *events = (epoll_event *)malloc(sizeof(epoll_event) * USER_LIMIT);
    int epollfd = epoll_create(USER_LIMIT);
    assert(epollfd != -1);
    addfd(epollfd, listenfd, true);
    connected--;
    while (1)
    {
        int ret = epoll_wait(epollfd, events, USER_LIMIT, -1);
        if (ret < 0)
        {
            printf("epoll failure\n");
            break;
        }
        et(events, ret, epollfd, listenfd);
    }
    close(listenfd);
    free(events);
    return 0;
}