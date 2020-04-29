#include <sys/socket.h>
//#include <sys/types.h>
#include <netinet/in.h>
//#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include <errno.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <locale.h>


void delete_current_string(int len)
{
    int i;
    for(i = 0; i < len + 2; i++)
    {
        printw("\b \b");
    }
}

int main(int argc, char *argv[])
{
	int sockfd = 0, n = 0;
	char recvBuff[1024];
    char message[1025];
    int messageLen = 0;
	struct sockaddr_in serv_addr;
    
    setlocale(LC_ALL, "");

	if(argc > 3)
	{
		printf("\n Usage: %s <ip of server> \n",argv[0]);
		return 1;
	}

	memset(recvBuff, '0',sizeof(recvBuff));

	/* a socket is created through call to socket() function */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

    int server_port = 5000;
    if(argc > 1)
    {
        server_port = atoi(argv[2]);
        if(server_port < 1000 || server_port >= 64000)
        {
            fprintf(stderr, "Bad port number: %s\n", argv[2]);
            return -1;
        }
    }    
	serv_addr.sin_port = htons(server_port);
    
	serv_addr.sin_family = AF_INET;

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	{
		printf("\n inet_pton error occured\n");
		return 1;
	}

	/* Information like IP address of the remote host and its port is
	 * bundled up in a structure and a call to function connect() is made
	 * which tries to connect this socket with the socket (IP address and port)
	 * of the remote host
	 */
	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return 1;
	}

    struct timeval tv;
    fd_set rfds;
    
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    
    
    initscr();
    keypad(stdscr, 1);

    /*
     * Register on server
     */
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    printw("%s", recvBuff);   
    refresh();
    
    getstr(message);
    messageLen = strlen(message);
    write(sockfd, message, messageLen);
    
    /*
     * Main cycle
     */
    noecho();
    halfdelay(1);
    
    int exit = 0;    
    
    printw("> ");
    refresh();
    
    messageLen = 0;
    message[messageLen] = 0;
    
    while(!exit)
    {
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        FD_SET(sockfd, &rfds);
        
        int ready = select(sockfd + 1, &rfds, NULL, NULL, &tv);
        
        if(ready > 0)
        {
            if(FD_ISSET(sockfd, &rfds))
            {
                n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
                recvBuff[n] = 0;
                delete_current_string(messageLen);
                printw("%s\n", recvBuff);
                printw("> %s", message);
                refresh();
            }
            
            if(FD_ISSET(0, &rfds))
            {
                int c = getch();
                
                switch(c)
                {
                    case KEY_F(2):
                        refresh();
                        write(sockfd, message, messageLen);
                        delete_current_string(messageLen);
                        messageLen = 0;
                        message[messageLen] = 0;
                        printw("> ");
                        refresh();
                        break;
                        
                    case KEY_BACKSPACE:
                        if(messageLen > 0)
                        {
                            printw("\b \b");
                            refresh();
                            messageLen--;
                            message[messageLen] = 0;
                        }
                        break;
                        
                    case KEY_F(10):
                        close(sockfd);
                        exit = 1;
                        break;
                        
                    default:
                        printw("%c", c);
                        refresh();
                        message[messageLen] = c;
                        messageLen++;
                        message[messageLen] = 0;
                        break;                
                }
            }
        }
    }
    
    endwin();

	return 0;
} 
 
