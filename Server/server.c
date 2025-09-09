#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
int main()
{
	//create socket address structure and initialize to zero	
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9877);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	
	//create socket
	int server_id;
	server_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//convert to listener and bind socket to socket address 
	bind(server_id, (struct sockaddr *) &servaddr, sizeof(servaddr));
	listen(server_id, 5);
	
	//check for connections
	int client_id;
	pid_t pid;
	for ( ; ; )
	{
		client_id = accept(server_id, NULL, NULL);
		
		//Concurrency	
		pid = fork();
		
		//check if child process
		if (pid == 0)
		{
			close(server_id);
			int n;
			char buffer[100];
			
			while( (n = read(client_id, buffer, sizeof(buffer))) > 0)
			{
				write(client_id, buffer, n);
			}
			close(client_id);
			exit(0);
		}
		
		//close client connection in parent
		close(client_id);
	}
	
	return 0;
}
