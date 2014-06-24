#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <netdb.h>
#include <arpa/inet.h>

int
main(int argc, char **argv)
{
    char    *ptr, **pptr;
    char    str [INET_ADDRSTRLEN];
    struct  hostent *hptr;

    ptr=argv[1];

    if ( (hptr = gethostbyname (ptr) ) == NULL)
    {
        printf ("gethostbyname error for host: %s: %s",
                ptr, strerror (h_errno) );
        return 1;
    }

    if (hptr->h_addrtype != AF_INET)
    {
        printf("unknown address type");
        return 1;
    }

    pptr = hptr->h_addr_list;
    char* p_servaddr = *pptr;
    for ( ; *pptr != NULL; pptr++)
        printf("\taddress: %s\n",
                inet_ntop(hptr->h_addrtype, *pptr, str, sizeof (str)));

    int sockfd;
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
         printf("socket error %s", strerror(errno));
         return 1;
    }

    if (connect(sockfd, (struct sockaddr*) p_servaddr, sizeof(*p_servaddr)) < 0)
    {
         printf("connect error %s", strerror(errno));
         return 1;
    }

    return 0;
}

