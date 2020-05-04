all: server client

server.o: server.cpp
	g++ -c server.cpp -o server.o
	
client.o: client.cpp
	g++ -c client.cpp -o client.o

server: server.o
	g++ server.o -o server.out
	
client: client.o
	g++ client.o -lncurses -o client.out

