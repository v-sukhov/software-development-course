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
#include <signal.h>

#include <unordered_map>
#include <unordered_set>


int main(int argc, char *argv[]) {

	int listenfd = 0, connfd = 0;
	char recvBuff[1024];
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

	std::unordered_map<int, std::string> users;
	std::unordered_set<int> noLoginUsers;

	auto connectUser = [&](int user) {
						   noLoginUsers.insert(user);
					   };

	auto sendMessage = [&](std::string msg) {
						   printf("send message: %s\n", msg.c_str());
						   for (auto it = users.begin(); it != users.end();) {
							   // printf("lkjaskldfjasd\n");
							   int user = it->first;

							   // printf("%ld\n%ld\n", strlen(msg.c_str()), msg.length());
							   int w = write(user, msg.c_str(), strlen(msg.c_str()));
							   // printf("%d\n", w);
							   
							   if (w > 0) {
								   // printf("aksdfa\n");
								   it++;
							   } else {
								   printf("disconnect user\n");
								   it = users.erase(it);
							   }
						   }
					   };

	auto loginUser = [&](int user, std::string name) {
						 users.insert({user, name});
						 noLoginUsers.erase(user);
						 sendMessage("login " + name);
					 };
	signal(SIGPIPE, SIG_IGN);
	
	while (true) {
		FD_ZERO(&rfds);
		FD_SET(listenfd, &rfds);

		int maxFD = listenfd;

		for (int i: noLoginUsers)
			/*printf("%d\n", i), */FD_SET(i, &rfds), maxFD = std::max(maxFD, i);

		for (std::pair<int, std::string> i: users)
			/* printf("%d\n", i.first), */ FD_SET(i.first, &rfds), maxFD = std::max(maxFD, i.first);


		printf("murmay\n");
		int cntRequest = select(maxFD + 1, &rfds, NULL, NULL, NULL);
		printf("unlock\n%ld\n%ld\n%d\n", noLoginUsers.size(), users.size(), cntRequest);

		if (FD_ISSET(listenfd, &rfds))
			connectUser(accept(listenfd, (struct sockaddr*)NULL, NULL));

		for (auto it = users.begin(); it != users.end();) {
			if (FD_ISSET(it->first, &rfds)) {
				printf("hello\n");
				int user = it->first;
				int n = read(user, recvBuff, sizeof(recvBuff) - 1);
				recvBuff[n] = 0;

				if (n > 0)
					sendMessage(it->second + ": " + std::string(recvBuff));
				it++;
			} else {
				it++;
			}
		}
		
		for (auto it = noLoginUsers.begin(); it != noLoginUsers.end();) {
			if (FD_ISSET(*it, &rfds)) {
				int user = *it;
				int n = read(user, recvBuff, sizeof(recvBuff) - 1);
				recvBuff[n] = 0;

				it++;
				loginUser(user, std::string(recvBuff));
			} else {
				it++;
			}
		}
		// printf("test\n");
	}
	
	return 0;
}
