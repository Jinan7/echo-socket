#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
int main(int argc, char **argv)
{
	//create socket address structure for server
	struct sockaddr_in servaddr;
	
	
	//create client socket
	int client_id;
	
	bzero(&servaddr, sizeof(servaddr));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9877);
	client_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(client_id, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	
	
	//echo logic
	char buffer[100];
	int stdineof, n, maxn;
	fd_set rset;
	
	stdineof = 0;
	FD_ZERO(&rset);
	
	for ( ; ; )
	{
		if (stdineof == 0)
			FD_SET(fileno(stdin), &rset);
		FD_SET(client_id, &rset);
		maxn = fileno(stdin) > client_id ? fileno(stdin) + 1: client_id + 1;
		select(maxn, &rset, NULL, NULL, NULL);
		
		if (FD_ISSET(client_id, &rset))
		{
			if ((n = read(client_id, buffer, 100))==0)
			{
				if (stdineof == 1)
				{
					break;
				}else{
					write(fileno(stdout), "Server terminated prematurely\n", sizeof("Server terminated prematurely\n"));
				}
				
				break;
			}
			write(fileno(stdout), buffer, n);
		}
		
		if (FD_ISSET(fileno(stdin), &rset))
		{
			if ((n=read(fileno(stdin), buffer, 100))==0)
			{
				stdineof = 1;
				shutdown(client_id, SHUT_WR);	
				FD_CLR(fileno(stdin),&rset);
				continue;
			}
			write(client_id, buffer, n);
		}
	}
	
	exit(0);
}
