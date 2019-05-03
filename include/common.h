#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <time.h>
#include <signal.h>
#include <sys/stat.h>


#define mff_RETURN_OK 1
#define mff_RETURN_ERROR -1


int mff_daemonize() ;
