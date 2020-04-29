#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	int sockfd = 0, n = 0;
	char recvBuff[1024];
	struct sockaddr_in serv_addr;

	int servPort = 5000;
	
	if (argc == 1 || argc > 3) {
		printf("Usage: %s <ip of server> <port>\n", argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "localhost") == 0)
		argv[1] = "127.0.0.1";

	if (argc == 3)
		servPort = atoi(argv[2]);

	memset(recvBuff, '0',sizeof(recvBuff));

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(servPort);

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
		printf("\n inet_pton error occured\n");
		return 1;
	}

	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\n Error : Connect Failed \n");
		return 1;
	}

	strcpy(recvBuff, "1234");
	
	write(sockfd, recvBuff, strlen(recvBuff));
	// printf("%ld", strlen(recvBuff));

	sleep(1);
	
	return 0;
}
