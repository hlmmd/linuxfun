#include <stdio.h>
#include "SocketfunConfig.h"
int main(int argc, char *argv[])
{
    printf("%d.%d\n",Socketfun_VERSION_MAJOR,Socketfun_VERSION_MINOR);
    if (argc < 2)
    {

        //  std::cout << "Usage: " << argv[0] << " number" << std::endl;
        return 1;
    }

    return 0;
}