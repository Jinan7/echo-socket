#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9877);

    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    int n;
    char sendline[100], recvline[100];

    int len = sizeof(servaddr);
    struct sockaddr_in *reply;
    reply = malloc(sizeof(struct sockaddr_in));
    while ( read(fileno(stdin), sendline, sizeof(sendline)) )
    {
    

        sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

        n = recvfrom(sockfd, recvline, sizeof(recvline), 0, (struct sockaddr*) reply, &len);

    
        recvline[n] = 0;
        write(fileno(stdout), recvline, n);
    }



    exit(0);
}