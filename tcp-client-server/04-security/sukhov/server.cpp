/* --- server.cpp --- */
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
#include <signal.h>

#include <unordered_map>
#include <vector>

#include "symmetric_security.h"
#include "dh_security.h"

using namespace std;

/* Struct for discribe connection
 */
struct client_info
{
    string name;
    string IP;
    dh_security_t dh;
    char sym_key[100];
    client_info(string _name, string _IP)
    {
        name = _name;
        IP = _IP;
        sym_key[0] = 0;
        dh_init_by_default(&dh);
    }
};


int build_dh_key_message(char *message, client_info &client)
{
    return dh_print_to_packet(message, &client.dh);
}

int process_dh_key_response(char *message, client_info &client)
{
    dh_update_from_packet(message, &client.dh);
}

void decript_message(char *message, int len, char *decriptedMessage, client_info &client)
{
    decript(client.sym_key, message, len, decriptedMessage);
}

void encript_message(char *message, int len, char *encriptedMessage, client_info &client)
{
    encript(client.sym_key, message, len, encriptedMessage);
}


int main(int argc, char *argv[])
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
    socklen_t len;
    char client_ip[INET_ADDRSTRLEN];

	char sendBuff[1025];
    char recvBuff[1025];
    char decriptedBuff[1025];
    char encriptedBuff[1025];
    dh_security_t dh_security;
	time_t ticks;

	/* creates an UN-named socket inside the kernel and returns
	 * an integer known as socket descriptor
	 * This function takes domain/family as its first argument.
	 * For Internet family of IPv4 addresses we use AF_INET
	 */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));
    memset(recvBuff, '0', sizeof(recvBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int server_port = 5000;
    if(argc > 1)
    {
        server_port = atoi(argv[1]);
        if(server_port < 1000 || server_port >= 64000)
        {
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
    
    unordered_map<int, client_info> clients_map;
    struct timeval tv;
    fd_set rfds;
    
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    len = sizeof(client_addr);
    
    signal(SIGPIPE, SIG_IGN);
    
    vector<string> messages;
    messages.reserve(100);

	while(1)
	{
		/* In the call to accept(), the server is put to sleep and when for an incoming
		 * client request, the three way TCP handshake* is complete, the function accept()
		 * wakes up and returns the socket descriptor representing the client socket.
		 */
        
        FD_ZERO(&rfds);
        FD_SET(listenfd, &rfds);
        
        int max_fd = listenfd;
        
        /*
         * Add all connections to rfds
         */
        for(unordered_map<int, client_info>::iterator itr = clients_map.begin(); itr != clients_map.end(); itr++)
        {
            FD_SET(itr->first, &rfds);
            max_fd = max(max_fd, itr->first);
        }
        
        int ready = select(max_fd + 1, &rfds, NULL, NULL, &tv);
        
        if(ready > 0)
        {
            if(FD_ISSET(listenfd, &rfds))
            {
                connfd = accept(listenfd, (struct sockaddr *)&client_addr, &len);
                
                struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client_addr;
                struct in_addr ipAddr = pV4Addr->sin_addr;
                inet_ntop( AF_INET, &ipAddr, client_ip, INET_ADDRSTRLEN );                  
                
                client_info client("",client_ip);
                
                int len = build_dh_key_message(sendBuff, client);
                
                
                //snprintf(sendBuff, sizeof(sendBuff), "Register: ");
                
                
                if(write(connfd, sendBuff, len) > 0)
                {
                    clients_map.insert({(int)connfd, client});
                }
                              
                ticks = time(NULL);
                printf("%.24s New client connected: %s \n", ctime(&ticks), client_ip);                
            }
            
            for(unordered_map<int, client_info>::iterator itr = clients_map.begin(); itr != clients_map.end(); itr++)
            {
                if(FD_ISSET(itr->first, &rfds))
                {
                    
                    int n = read(itr->first, recvBuff, sizeof(recvBuff) - 1);
                    recvBuff[n] = 0;
                    
                    if(n > 0)
                    {
                    
                        if(itr->second.sym_key[0] == 0)
                        {
                            process_dh_key_response(recvBuff, itr->second);
                            dh_get_symmetric_key(itr->second.sym_key, &itr->second.dh);
                            snprintf(sendBuff, sizeof(sendBuff), "Register: ");
                            write(connfd, sendBuff, strlen(sendBuff));
                            printf("Recieved key from client %s: %s\n", itr->second.IP.c_str(), itr->second.sym_key);
                        }
                        else if(itr->second.name.length() == 0)
                        {
                            decript_message(recvBuff, n, decriptedBuff, itr->second);
                            ticks = time(NULL);
                            printf("%.24s Client registered: %s for %s \n", ctime(&ticks), decriptedBuff, itr->second.IP.c_str());
                            itr->second.name = decriptedBuff;
                        }
                        else
                        {
                            decript_message(recvBuff, n, decriptedBuff, itr->second);
                            
                            sprintf(sendBuff, "%s: %s", itr->second.name.c_str(), decriptedBuff);
                            
                            messages.push_back(sendBuff);
                        
                            ticks = time(NULL);
                            printf("%.24s %s : %s \n", ctime(&ticks), itr->second.name.c_str(), decriptedBuff);
                            //printf("encripted : %s \n", recvBuff);
                        }
                    }
                }
            }
            
            while(messages.size() > 0)
            {
                string sendMessage = messages.back();
                messages.pop_back();
                strcpy(sendBuff, sendMessage.c_str());
                int len = sendMessage.length();
                
                unordered_map<int, client_info>::iterator itr = clients_map.begin();
                while(itr != clients_map.end())
                {
                    if(itr->second.name.length() > 0)
                    {
                        encript_message(sendBuff, len, encriptedBuff, itr->second);
                        if(write(itr->first, encriptedBuff, len) > 0)
                        {
                            itr++;
                        }
                        else
                        {
                            close(itr->first);
                            itr = clients_map.erase(itr);
                        }
                    }
                }                
            }

        }
	}
}

 
