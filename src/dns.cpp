#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

#include <netdb.h>
#include <arpa/inet.h>

#include <iostream>
#include <ostream>

#include <list>
#include <string>
#include <algorithm>

using namespace std;

typedef pair<string,double> pair_type;
list< pair_type > tm_lst;


void tm_probe(string str)
{

    struct timeval tv={0};
    gettimeofday(&tv,NULL);

    tm_lst.push_back(make_pair(str,tv.tv_sec + tv.tv_usec / 1000000.0));
}



int
main(int argc, char **argv)
{
    char    **pptr;
    char    str [INET_ADDRSTRLEN];
    struct  hostent *hptr;

    char host[] ="82.165.27.2";

    char sbuf[] =
        "GET /SiteOptimiser/image-200KB.bmp?id=1 HTTP/1.1\r\n"
        "User-Agent: curl/7.36.0\r\n"
        "Host: 82.165.27.2\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n\r\n";
 
    char rbuf[4096]={0};

    struct sockaddr_in servaddr;
    memset(&servaddr,0, sizeof(servaddr));

    int sockfd;
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
         printf("socket error %s", strerror(errno));
         return 1;
    }
    
    tm_probe("Start");
    if ( (hptr = gethostbyname (host) ) == NULL)
    {
        printf ("gethostbyname error for host: %s: %s",
                host, strerror (h_errno) );
        return 1;
    }
    tm_probe("DNS");

    if (hptr->h_addrtype != AF_INET)
    {
        printf("unknown address type");
        return 1;
    }

    pptr = hptr->h_addr_list;
    for ( ; *pptr != NULL; pptr++)
        printf("\taddress: %s\n",
                inet_ntop(hptr->h_addrtype, *pptr, str, sizeof (str)));


    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(80);
    memcpy(&servaddr.sin_addr, hptr->h_addr_list[0], hptr->h_length);

    if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
    {
         printf("connect error %s", strerror(errno));
         return 1;
    }
    tm_probe("Connect");
    send(sockfd, sbuf, strlen(sbuf),0);
    tm_probe("Sent");

    bool first_byte = true;

    while(recv(sockfd,rbuf,4096,0))
    {
        if(first_byte)
        {
            tm_probe("Start");
            first_byte=false;
        }
        //printf("%s",rbuf);
    }

    tm_probe("Context");

    double tm_start = tm_lst.begin()->second;
    for_each(tm_lst.begin(),tm_lst.end(),
            [&tm_start](const pair_type& elem)
            {
                cout << fixed << elem.first << "\t" << ":" << "\t" <<
                elem.second << "\t" << (int)round((elem.second - tm_start) * 1000) << endl;
            }
    );


    return 0;
}

