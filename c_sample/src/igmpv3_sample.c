

/*
Definition:
http://nccur.lib.nccu.edu.tw/bitstream/140.119/36906/6/35903006.pdf

1.Please set IGMP to 3, echo 3 >/proc/sys/net/ipv4/conf/eth0/force_igmp_version
2.run sample app.
3.If you want to check IGMP, you can cat /proc/net/igmp & /proc/net/mcfilter. You can run Wireshark to check packet!

Kernel: Networking options
  [*]   IP: multicast routing                                    
  ¢x ¢x     [ *]     IP: PIM-SM version 1 support (NEW)               
  ¢x ¢x     [ *]     IP: PIM-SM version 2 support (NEW)                 

  Sample:
The code is form http://isec.pl/vulnerabilities/isec-0018-igmp.txt. It's fail whan I make, so I modify it a little.
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/types.h>
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define MCAST_INCLUDE            1
#define IP_MSFILTER              41
#define IP_UNBLOCK_SOURCE        37
#define IP_BLOCK_SOURCE          38

void fatal (const char *message)
{
    printf ("\n");
	if (!errno)
	{
		fprintf (stdout, "FATAL: %s\n", message);
	}
	else
	{
		fprintf (stdout, "FATAL: %s (%s) ", message,
	   (char *) (strerror (errno)));
	}
    printf ("\n");
    fflush (stdout);
    exit (1);
}


int main(int argc, char *argv[])
{
    int s, r, l;
    struct ip_mreqn mr;
    struct ip_mreq_source ms;

    s = socket (AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
        fatal ("socket");

	//      first join mcast group
    memset (&mr, 0, sizeof (mr));
    mr.imr_multiaddr.s_addr = inet_addr ("224.0.0.199");
    l = sizeof (mr);
    r = setsockopt (s, SOL_IP, IP_ADD_MEMBERSHIP, &mr, l);
    if (r < 0)
		fatal ("setsockopt");
       
    //      add source filter count=1
    memset (&ms, 0, sizeof (ms));
    ms.imr_multiaddr.s_addr = inet_addr ("224.0.0.199");
    ms.imr_sourceaddr.s_addr = inet_addr ("1.1.1.1");
    l = sizeof (ms);
    r = setsockopt (s, SOL_IP, IP_BLOCK_SOURCE, &ms, l);
    if (r < 0)
		fatal ("setsockopt1");

    memset (&ms, 0, sizeof (ms));
    ms.imr_multiaddr.s_addr = inet_addr ("224.0.0.199");
    ms.imr_sourceaddr.s_addr = inet_addr ("2.1.1.2");
    l = sizeof (ms);
    r = setsockopt (s, SOL_IP, IP_BLOCK_SOURCE, &ms, l);
    if (r < 0)
		fatal ("setsockopt2");       

    printf("wait \n");
    getchar ();

    return 0;
}


