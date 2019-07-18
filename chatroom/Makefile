target4=client_epoll
all: server client server_epoll $(target4)


$(target4):$(target4).cpp
	g++ -o $(target4) $(target4).cpp

server:server.cpp
	g++ -o server server.cpp 

server_epoll:server_epoll.cpp
	g++ -o server_epoll server_epoll.cpp 
client: client.cpp
	gcc -o client client.cpp

.PHONY:clean
clean:
	rm -rf client server