#include "tools.cpp"

WINDOW *chatBorder, *chat, *input;

void printMessage(char *msg) {
	int x, y;
	getyx(chat, y, x);
	
	wprintw(chat, "%s", msg);
	getyx(chat, y, x);

	wmove(chat, y + 1, 1);
	
	wrefresh(chat);
	if (y == LINES - 4) {
		wmove(chat, 0, 0);
		wdeleteln(chat);
		wmove(chat, y, 1);
	} 
}

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "");
	
	int sockfd = 0;
	struct sockaddr_in serv_addr;
	
	if (argc == 1 || argc > 3) {
		std::cout << "Usage: %s <ip of server> <port>\n", argv[0];
		return 1;
	}
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cout << "\n Error : Could not create socket \n";
		return 1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	{ // update port
		int port = get_port(argc, argv, true);
		if(port == -1){
			std::cout << "\nBad port. Usage port 1000-64000\n";
			return 1;
		}
		serv_addr.sin_port = htons(port);
	}

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
		std::cout << "\n inet_pton error occured\n";
		return 1;
	}

	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		std::cout <<"\n Error : Connect Failed \n";
		return 1;
	}

	// task 3
	
	
	char login[100];
	{// authorization
		std::cout << "Write your login: ";std::cout.flush();
		scanf("%s", login);

		write(sockfd, login, sizeof(login)); // message to server	
	}

	{ // ncurses
		if(!initscr()){
			std::cout << "\nERROR: initscr was not initialized\n";
			return 1;
		}
		refresh();
		mvaddstr(0, (COLS - strlen("Messenger")) / 2, "Messenger");
		chatBorder = newwin(LINES - 3, COLS - 1, 1, 1);
		chat = newwin(LINES - 3, COLS - 2, 2, 2);
		box(chatBorder, 0, 0);
		wmove(chatBorder, 0, 1);
		wmove(chat, 0, 1);
		
		input = newwin(1, COLS, LINES - 1, 0);
		wprintw(input, "%s > ", login);


		wrefresh(chat);
		wrefresh(input);
		wrefresh(chatBorder);
		noecho();
		refresh();
	}
	
	fd_set rfds;
	FD_ZERO(&rfds);
	char recBuff[1024];
	
	int x, y;
	getyx(input, y, x);
	wmove(input, y, x);
	wrefresh(input);

	int lenMsg = 0;
	char msg[1025];

	while (true) {
		FD_SET(0, &rfds);	
		FD_SET(sockfd, &rfds);

		select(sockfd + 1, &rfds, NULL, NULL, NULL);

		if (FD_ISSET(0, &rfds)) {
			char x = getch();
			if (x == 10) { // enter
				int x, y;
				getyx(input, y, x);
				wmove(input, y, strlen(login) + 3);
				wclrtoeol(input);
				wrefresh(input);

				msg[lenMsg] = 0;

				write(sockfd, msg, strlen(msg));
				
				lenMsg = 0;
			} 
			else if (x == 127) { // backspace
				int x, y;
				getyx(input, y, x);
				if (x > strlen(login) + 3) {
					wmove(input, y, x - 1);
					wprintw(input, " ");
					wmove(input, y, x - 1);
					lenMsg--;
				}
			} 
			else{ // update this bug
				wprintw(input, "%c", x);
				msg[lenMsg++] = x;
			}
		}
		if (FD_ISSET(sockfd, &rfds)) {
			int n = read(sockfd, recBuff, 1024);
			recBuff[n] = 0;
			printMessage(recBuff);
		}
		wrefresh(input);
	}

    delwin(chat);
	delwin(chatBorder);
	delwin(input);
    endwin();
	return 0;
}