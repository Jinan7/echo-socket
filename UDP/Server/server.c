#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

int main()
{
    //create socket address structure
    struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9877);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //create socket
    int sockfd;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("Socket create with id %d\n", sockfd);

    bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    int len = sizeof(cliaddr),n;
    char mesg[100];
    for ( ; ; )
    {
        n = recvfrom(sockfd, mesg, sizeof(mesg),0, (struct sockaddr*) &cliaddr, &len);
        sendto(sockfd, mesg, strlen(mesg), 0, (struct sockaddr*) &cliaddr, len);
    }

    exit(0);

}