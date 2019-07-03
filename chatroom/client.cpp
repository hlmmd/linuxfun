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

#define BUFFER_SIZE 64

int main( int argc,char ** argv)
{
    if(argc<=2)
    {
        printf("usage: ./%s ip_address port \n",basename(argv[0]));
        return 1;
    }
    return 0;
}