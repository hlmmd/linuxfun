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

#define USER_LIMIT 1000 /* 最大用户数  */
#define BUFFER_SIZE 64  //  缓冲区大小
#define FD_LIMIT 65535  // fd限制

using namespace std;

struct client_data
{
    struct sockaddr_in client_address;
    int sockfd;
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

    vector<client_data> users(USER_LIMIT);

    for (int i = 0; i < users.size(); i++)
    {
        //设置为-1表示未使用
        users[i].sockfd = -1;
    }

    unordered_map<int, int> user_map;

    int current_users = 0;
    int user_index = 0;

    while (1)
    {
        fd_set rfds;
        int maxfd = listenfd;
        //清空集合
        FD_ZERO(&rfds);
        //将当前连接和listen socket句柄加入到集合中
        FD_SET(listenfd, &rfds);

        for (auto it = user_map.begin(); it != user_map.end(); it++)
        {
            int userfd = users[it->second].sockfd;
            FD_SET(userfd, &rfds);
            maxfd = maxfd > userfd ? maxfd : userfd;
        }

        ret = select(maxfd + 1, &rfds, NULL, NULL, NULL);
        if (ret == -1)
        {
            printf("select error\n");
            break;
        }
        else if (ret == 0)
            continue;
        else
        {
            //listenfd有可读事件，说明有新的连接，使用accept进行处理
            if (FD_ISSET(listenfd, &rfds))
            {
                struct sockaddr_in client_addr;
                socklen_t client_addr_length = sizeof(client_addr);
                int clientfd;
                if ((clientfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_length)) > 0)
                {
                    if (current_users >= USER_LIMIT)
                    {
                        close(clientfd);
                        continue;
                    }

                    printf("Connected from %s\n", inet_ntoa(client_addr.sin_addr));
                    int unused = -1;
                    for (int i = 0; i < users.size(); i++)
                    {
                        if (users[i].sockfd == -1)
                        {
                            unused = i;
                            break;
                        }
                    }
                    user_map[user_index] = unused;
                    users[unused].sockfd = clientfd;
                    current_users++;
                    user_index++;
                }
                printf("current user number: %d\n", current_users);
            }

            for (auto it = user_map.begin(); it != user_map.end();)
            {
                auto user_data = users[it->second];
                //int userfd = users[it->second].sockfd;
                bool deleteflag = false;
                if (FD_ISSET(user_data.sockfd, &rfds))
                {
                    bzero(user_data.buffer, BUFFER_SIZE);
                    int len = recv(user_data.sockfd, user_data.buffer, BUFFER_SIZE, 0);
                    if (len > 0)
                    {
                        printf("recv from client[%d]: %s\n", user_data.sockfd, user_data.buffer);
                    }
                    else if (len == 0)
                    {
                        printf("client %d disconnected!\n", user_data.sockfd);
                        user_data.sockfd = -1;
                        current_users--;
                        printf("current user number: %d\n", current_users);
                        deleteflag = true;
                        auto tempit = it;
                        auto nextit = ++it;
                        user_map.erase(tempit);
                        it = nextit;
                    }
                }
                if (deleteflag == false)
                    it++;
            }
        }
    }

    //   free(users);
    close(listenfd);

    return 0;
}