#include "tools.cpp"


int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "");

	int listenfd = 0;
	char recvBuff[1024];
	struct sockaddr_in serv_addr;

	char sendBuff[1025];

	if (argc > 2) {
		std::cout <<"\nUsage " << argv[0] << " <port>\n";
		return 1;
	}

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	{ // update port
		int port = get_port(argc, argv, false);
		if(port == -1){
			std::cout << "\nBad port. Usage port 1000-64000\n";
			return 1;
		}
		serv_addr.sin_port = htons(port);
	}

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(listenfd, 10);

	fd_set rfds;


	// task 3
	std::unordered_map<int, std::string> users;
	std::unordered_set<int> noLoginUsers;

	auto connectUser = [&](int user) {
		noLoginUsers.insert(user);
	};

	auto sendMessage = [&](std::string msg) {
		printf("send message: %s\n", msg.c_str());
		for (auto it = users.begin(); it != users.end();) {
			int user = it->first;

			int w = write(user, msg.c_str(), strlen(msg.c_str()));
			
			if (w > 0) {
				it++;
			} 
			else {
				printf("disconnect user %s\n", it->second.c_str());
				it = users.erase(it);
			}
		}
	};

	auto loginUser = [&](int user, std::string name) {
		printf("login user %s\n", name.c_str());
		users.insert({user, name});
		noLoginUsers.erase(user);
		sendMessage("login " + name);
	};
	
	while (true) {
		FD_ZERO(&rfds);
		FD_SET(listenfd, &rfds);

		int maxFD = listenfd;

		for (int i: noLoginUsers){
			FD_SET(i, &rfds), maxFD = std::max(maxFD, i);
		}

		for (std::pair<int, std::string> i: users){
			FD_SET(i.first, &rfds), maxFD = std::max(maxFD, i.first);
		}

		select(maxFD + 1, &rfds, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rfds)){
			connectUser(accept(listenfd, (struct sockaddr*)NULL, NULL));
		}

		for (auto it = users.begin(); it != users.end();) {
			if (FD_ISSET(it->first, &rfds)) {
				int user = it->first;
				int n = read(user, recvBuff, sizeof(recvBuff) - 1);
				recvBuff[n] = 0;

				if (n > 0){
					sendMessage(it->second + ": " + std::string(recvBuff)), it++;
				}
				else{
					printf("disconnect user %s\n", it->second.c_str()), it = users.erase(it);
				}
			} 
			else {
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
				
			} 
			else {
				it++;
			}
		}
	}
	
	return 0;
}