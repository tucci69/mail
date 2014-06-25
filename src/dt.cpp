#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 4096


int
main(int argc, char **argv)
{
    int     sockfd, n;
    char    recvline[ MAXLINE +1 ];
    struct sockaddr_in servaddr;

    if (argc != 2)
    {
        printf("usage: %s <IPaddress>", argv[0] );
        exit(1);
    }

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
         printf("socket error %s", strerror(errno));

         exit(1);
    }

    memset(&servaddr,0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);  /* daytime server */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
         printf("inet_pton error for %s", argv[1]);
         exit(1);
    }

    if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
    {
         printf("connect error");
         exit(1);
    }

    int cnt=0;

    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        if(n) cnt++;
        recvline[n] = 0;        /* null terminate */
        if (fputs(recvline, stdout) == EOF)
        {
            printf("fputs error");
            exit(1);
        }

    }
    
    printf( "Numbers od n>0 is %d", cnt );

    if (n < 0)
    {
         printf("read error");
         exit(1);
    }

    exit(0);
}
             
