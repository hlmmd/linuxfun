all: server client

server:server.cpp
	g++ -o server server.cpp 

client: client.cpp
	gcc -o client client.cpp

.PHONY:clean
clean:
	rm -rf client server