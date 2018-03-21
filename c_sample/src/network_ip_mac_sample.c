#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define MAX_SHORT_LENGTH        128

int get_mac_address(char *name, char *mac)
{
	int ret = -1;
	struct ifreq s;
	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

	strcpy(s.ifr_name, name);
	if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) 
	{
		sprintf(mac, "%02x-%02x-%02x-%02x-%02x-%02x",
		(unsigned char) s.ifr_addr.sa_data[0],
		(unsigned char) s.ifr_addr.sa_data[1],
		(unsigned char) s.ifr_addr.sa_data[2],
		(unsigned char) s.ifr_addr.sa_data[3],
		(unsigned char) s.ifr_addr.sa_data[4],
		(unsigned char) s.ifr_addr.sa_data[5]);
		ret = 0;
	}

	return ret;
}

int get_ip_address(char *name, char *ip)
{
	int fd;
	int ret = -1;
	struct ifreq ifr;
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd)
	{
		ifr.ifr_addr.sa_family = AF_INET;
		strncpy(ifr.ifr_name, name, IFNAMSIZ-1);
		ioctl(fd, SIOCGIFADDR, &ifr);
		strcpy(ip, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
		close(fd);
		ret = 0;
	}
	
	return ret;
}


int main(int argc, char *argv[])
{
	char ip[MAX_SHORT_LENGTH];
	char mac[MAX_SHORT_LENGTH];
	
	get_ip_address("eth0", ip);		
	get_mac_address("eth0", mac);
	
	printf("ip : %s, mac : %s\n", ip, mac);

    return 0;
}


