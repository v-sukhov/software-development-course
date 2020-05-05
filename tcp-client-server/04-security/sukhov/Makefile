all: server client

dh_security.o: dh_security.c dh_security.h
	g++ -c dh_security.c -o dh_security.o
	
symmetric_security.o: symmetric_security.c symmetric_security.h
	g++ -c symmetric_security.c -o symmetric_security.o

server.o: server.cpp 
	g++ -c server.cpp -o server.o
	
client.o: client.c
	g++ -c client.c -o client.o

server: server.o symmetric_security.o dh_security.o
	g++ symmetric_security.o dh_security.o server.o -o server.out
	
client: client.o symmetric_security.o dh_security.o
	g++ client.o symmetric_security.o dh_security.o -lncursesw -o client.out
	
security_test: dh_security.c dh_security.h
	g++ dh_security.c -o dh_security_test.out

