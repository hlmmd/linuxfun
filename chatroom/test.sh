#!/bin/bash

for ((i=0;i<100;i++));do
{

#sleep 0.01
#nc 127.0.0.1 6000 &
./client_epoll 127.0.0.1 6000 1000 &

}
done
