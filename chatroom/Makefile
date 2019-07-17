all: server client server_epoll

server:server.cpp
	g++ -o server server.cpp 

server_epoll:server_epoll.cpp
	g++ -o server_epoll server_epoll.cpp 
client: client.cpp
	gcc -o client client.cpp

.PHONY:clean
clean:
	rm -rf client server