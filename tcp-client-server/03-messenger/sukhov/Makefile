all: server client

server.o: server.cpp
	g++ -c server.cpp -o server.o
	
client.o: client.c
	gcc -c client.c -o client.o

server: server.o
	g++ server.o -o server.out
	
client: client.o
	gcc client.o -lncursesw -o client.out

