# linuxfun
linux programming
# wait for commit

## #define _GNU_SOURCE 1

定义了_GNU_SOURCE相当于开启了对所有特性的支持（ANSI, ISO, POSIX, FIPS等）

例如basename函数。

注意：再定义头文件前定义。

## listen函数 backlog参数

listen后会维护一个队列，存放着已经建立连接（完成三次握手）的套接字，等待accept。当队列大小达到backlog时，之后的链接请求会拒绝或重传

```
The backlog argument defines the maximum length to which the  queue  of
pending  connections  for  sockfd  may  grow.   If a connection request
arrives when the queue is full, the client may receive an error with an
indication  of  ECONNREFUSED  or,  if  the underlying protocol supports
retransmission, the request may be ignored so that a later reattempt at
connection succeeds.
```

