#include <stdio.h>
#include "SocketfunConfig.h"
#include "common.h"

int main(int argc, char *argv[])
{
    printf("%d.%d\n", Socketfun_VERSION_MAJOR, Socketfun_VERSION_MINOR);
    if (argc < 2)
    {

        //  std::cout << "Usage: " << argv[0] << " number" << std::endl;
        //    return 1;
    }

    mff_daemonize();

    //execl(argv[1],argv[1],NULL);

    // printf("%d.%d\n", Socketfun_VERSION_MAJOR, Socketfun_VERSION_MINOR);

    return 0;
}