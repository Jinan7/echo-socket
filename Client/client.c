#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	//create socket address structure for server
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9877);
	
	int client_id;
	client_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(client_id, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	exit(0);
}
