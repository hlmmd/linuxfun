#include "common.h"

//将当前进程变成守护进程。fork两次
int mff_daemonize()
{
    int pid;
    pid = fork();
    if (pid > 0)
        exit(0);
    else if (pid < 0)
        exit(1);
    //创建会话期
    setsid();

    //fork 两次
    pid = fork();
    if (pid > 0)
        exit(0);
    else if (pid < 0)
        exit(1);

    //设置工作目录，设置为/tmp保证具有权限
    chdir("/tmp");

    //设置权限掩码
    umask(0);

    //关闭已经打开的文件描述符
    for (int i = 0; i < getdtablesize(); i++)
        close(i);

    //忽略SIGCHLD信号，防止产生僵尸进程
    signal(SIGCHLD, SIG_IGN);
    
    return mff_RETURN_OK;
}