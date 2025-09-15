#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
	{
		printf("client %d closed connection\n", pid);
	}
	
	
	return;
}
int main()
{
	
	signal(SIGCHLD, sig_chld);
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
		printf("waiting for connection\n");
		client_id = accept(server_id, NULL, NULL);
		printf("Connection accepted\n");
		//Concurrency	
		pid = fork();
		
		//check if child process
		if (pid == 0)
		{
			close(server_id);
			int n;
			long arg1, arg2, res;
			char buffer[100];
			printf("Child processes running\n");
			while( (n = read(client_id, buffer, sizeof(buffer))) > 0)
			{
				sscanf(buffer, "%ld %ld", &arg1, &arg2);
				res = arg1 + arg2;
				printf("ready\n");
				snprintf(buffer, sizeof(buffer), "%ld\n", res);
				write(client_id, buffer, strlen(buffer));
				
			}
			
			
			close(client_id);
			exit(0);
		}
		
		
		//close client connection in parent
		close(client_id);
	}
	
	return 0;
}
