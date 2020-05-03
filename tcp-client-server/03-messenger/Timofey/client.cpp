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
#include <ncurses.h>

WINDOW *chat, *input;

void printMessage(char *msg) {
	int x, y;
	getyx(chat, y, x);
	wprintw(chat, "%s", msg);
	wmove(chat, y + 1, x);
	wrefresh(chat);
}

int main(int argc, char *argv[]) {
	int sockfd = 0;
	struct sockaddr_in serv_addr;

	int servPort = 5000;
	
	if (argc == 1 || argc > 3) {
		printf("Usage: %s <ip of server> <port>\n", argv[0]);
		return 1;
	}

	char *servIP;
	if (strcmp(argv[1], "localhost") == 0) {
		servIP = new char[sizeof("127.0.0.1")], strcpy(servIP, "127.0.0.1");
	} else {
		servIP = new char[sizeof(argv[1])], strcpy(servIP, argv[1]);
	}
	
	if (argc == 3) {
		servPort = atoi(argv[2]);
	}
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(servPort);

	if(inet_pton(AF_INET, servIP, &serv_addr.sin_addr) <= 0) {
		printf("\n inet_pton error occured\n");
		return 1;
	}

	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\n Error : Connect Failed \n");
		return 1;
	}

	printf("Write your name: ");
	fflush(stdout);

	char name[15];
	scanf("%s", name);

	write(sockfd, name, sizeof(name));

	initscr();
    refresh();
	
	chat = newwin(LINES - 1, COLS, 0, 0);
	box(chat, 0, 0);
	wmove(chat, 0, 1);
	wprintw(chat, "chat");
	wmove(chat, 1, 2);
	
	input = newwin(1, COLS, LINES - 1, 0);
	wprintw(input, "%s > ", name);

	wrefresh(chat);
	wrefresh(input);

	printMessage("help");

	fd_set rfds;
	FD_ZERO(&rfds);

	char recBuff[1025];
	noecho();

	
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
			if (x == 10) { // RETURN
				int x, y;
				getyx(input, y, x);
				wmove(input, y, strlen(name) + 3);
				wclrtoeol(input);
				wrefresh(input);

				msg[lenMsg] = 0;

				write(sockfd, msg, strlen(msg));
				
				lenMsg = 0;
			} else {
				wprintw(input, "%c", x);
				msg[lenMsg++] = x;
			}
		}
		if (FD_ISSET(sockfd, &rfds)) {
			int n = read(sockfd, recBuff, 1024);
			recBuff[n] = 0;
			printMessage(recBuff);
		}
		int x, y;
		getyx(input, y, x);
		wmove(input, y, x);
		wrefresh(input);
	}
    delwin(chat);
	delwin(input);
    endwin();
	
	return 0;
}
