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
	
	input = newwin(1, COLS, LINES - 1, 0);
	wprintw(input, "%s > ", name);

	wrefresh(chat);
	wrefresh(input);
	
    getch();

    delwin(chat);
	delwin(input);
    endwin();
	
	return 0;
}
