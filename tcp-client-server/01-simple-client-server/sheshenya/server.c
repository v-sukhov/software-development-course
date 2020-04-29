/* --- server.c --- */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <errno.h>
#include <string.h>
//#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[]) {
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
    int len;
    char str[INET_ADDRSTRLEN];

	char sendBuff[1025];
	time_t ticks;

	/* creates an UN-named socket inside the kernel and returns
	 * an integer known as socket descriptor
	 * This function takes domain/family as its first argument.
	 * For Internet family of IPv4 addresses we use AF_INET
	 */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /*
     * *
     * Task 4
     * *
     */
    int server_port = 5000;
    if (argc > 1) {
        server_port = atoi(argv[1]);
        if (server_port < 1000 || server_port >= 64000) {
            fprintf(stderr, "Bad port number: %s\n", argv[1]);
            return -1;
        }
    }
	serv_addr.sin_port = htons(server_port);

	/* The call to the function "bind()" assigns the details specified
	 * in the structure 『serv_addr' to the socket created in the step above
	 */
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	/* The call to the function "listen()" with second argument as 10 specifies
	 * maximum number of client connections that server will queue for this listening
	 * socket.
	 */
	listen(listenfd, 10);

	while (1) {
		/* In the call to accept(), the server is put to sleep and when for an incoming
		 * client request, the three way TCP handshake* is complete, the function accept()
		 * wakes up and returns the socket descriptor representing the client socket.
		 */
        len = sizeof(client_addr);
		connfd = accept(listenfd, (struct sockaddr *)&client_addr, &len);

		/* As soon as server gets a request from client, it prepares the date and time and
		 * writes on the client socket through the descriptor returned by accept()
		 */
		ticks = time(NULL);
		snprintf(sendBuff, sizeof(sendBuff), "%.24s\n", ctime(&ticks));
		write(connfd, sendBuff, strlen(sendBuff));
        
        /*
         * *
         * Task 5
         * *
         */

		/*
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client_addr;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
        printf("%s: %s", str, sendBuff);

		close(connfd);
		*/
		sleep(1);
	}
}

