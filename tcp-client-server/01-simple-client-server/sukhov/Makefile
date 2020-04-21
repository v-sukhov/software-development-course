all: server client

server.o: server.c
	gcc -c server.c -o server.o
	
client.o: client.c
	gcc -c client.c -o client.o

server: server.o
	gcc server.o -o server.out
	
client: client.o
	gcc client.o -o client.out

