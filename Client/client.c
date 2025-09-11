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
	char sendline[100], recvline[100];
	int stdineof, n, maxn;
	fd_set rset;
	
	
	FD_ZERO(&rset);
	maxn = fileno(stdin) > client_id ? fileno(stdin) + 1: client_id + 1;
	for ( ; ; )
	{
		FD_SET(fileno(stdin), &rset);
		FD_SET(client_id, &rset);
		
		select(maxn, &rset, NULL, NULL, NULL);
		
		if (FD_ISSET(client_id, &rset))
		{
			if (read(client_id, recvline, 100)==0)
			{
				fputs("Server terminated prematurely", stdout);
				break;
			}
			fputs(recvline, stdout);
		}
		
		if (FD_ISSET(fileno(stdin), &rset))
		{
			if (fgets(sendline, 100, stdin)==NULL)
			{
				break;	
			}
			write(client_id, sendline, strlen(sendline));
		}
	}
	
	exit(0);
}
