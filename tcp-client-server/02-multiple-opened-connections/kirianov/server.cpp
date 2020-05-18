
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unordered_set>
#include <signal.h>

using namespace std;

int main(int argc, char *argv[])
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
    	socklen_t len;
    	char str[INET_ADDRSTRLEN];

	char sendBuff[1025];
	time_t ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(listenfd, 10);
	unordered_set<int> users;
	struct timeval tv;
    	fd_set rfds;
    
    	tv.tv_sec = 0;
    	tv.tv_usec = 0;
    
   	len = sizeof(client_addr);
    
    	signal(SIGPIPE, SIG_IGN);
	while(1){
		FD_ZERO(&rfds);
		FD_SET(listenfd, &rfds);
		
		int max_fd = listenfd;
		
		int ready = select(max_fd + 1, &rfds, NULL, NULL, &tv);
		
		if(ready > 0)
		{
		    if(FD_ISSET(listenfd, &rfds))
		    {
		        connfd = accept(listenfd, (struct sockaddr *)&client_addr, &len);
		        users.insert(connfd);
		        
		        ticks = time(NULL);
		        snprintf(sendBuff, sizeof(sendBuff), "%.24s\n", ctime(&ticks));
		        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client_addr;
		        struct in_addr ipAddr = pV4Addr->sin_addr;
		        inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
		        printf("%s: %s", str, sendBuff);                
		    }
		}
		ticks = time(NULL);
		snprintf(sendBuff, sizeof(sendBuff), "%.24s\n", ctime(&ticks));
		
		unordered_set<int>::iterator it = users.begin();
		while(it != users.end()){
		    if(write(*it, sendBuff, strlen(sendBuff)) > 0){
		        ++it;
		    }
		    else{
		        close(*it);
		        it = users.erase(it);
		    }
		}
		sleep(1);
	}
}


