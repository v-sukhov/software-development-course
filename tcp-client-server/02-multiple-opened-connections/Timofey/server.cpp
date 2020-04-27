// #include <сstdio>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// #include <stdio>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#include <set>
#include <unordered_set>
// #include <iostream>

// using namespace std;

int main(int argc, char *argv[]) {
	int listenfd = 0, connfd = 0;
    char str[INET_ADDRSTRLEN];
	struct sockaddr_in serv_addr;
	int servPort = 5000;
	time_t ticks;

	char sendBuff[1025];

	if (argc > 2) {
		printf("Usage %s <port>\n", argv[0]);
		return 1;
	}

	if (argc == 2) {
		servPort = atoi(argv[1]);

		if (servPort < 1000 || servPort >= 50000) {
			printf("Bad port\n");
			return 1;
		}
	}

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(servPort);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(listenfd, 10);

	fd_set rfds;
	struct timeval tv;
	int retval;
	FD_ZERO(&rfds);
	FD_SET(listenfd, &rfds);
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	std::set<int> users;
	struct sockaddr_in client_addr;
			
	socklen_t len = sizeof(client_addr);
	
	while (true) {
		FD_ZERO(&rfds);
		FD_SET(listenfd, &rfds);
		
		while(select(listenfd + 1, &rfds, NULL, NULL, &tv)) {
			connfd = accept(listenfd, (struct sockaddr*)&client_addr, &len);
			users.insert(connfd);

			struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client_addr;
			struct in_addr ipAddr = pV4Addr->sin_addr;
			inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
			printf("connect: %s\n", str);

			
			FD_ZERO(&rfds);
			FD_SET(listenfd, &rfds);
		}

		ticks = time(0);
		snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));

		for (auto it = users.begin(); it != users.end();) {
			int user = *it;
			if (write(user, sendBuff, strlen(sendBuff)) > 0) {
				it++;
			} else {
				printf("disconnect user\n");
				it = users.erase(it);
			}
		}
		sleep(5);
	}
    return 0;
}
