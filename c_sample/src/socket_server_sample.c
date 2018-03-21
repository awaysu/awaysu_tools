#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 


int socket_server(int port)
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
    char buffer[1024];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));   

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
	
	memset(buffer, '0', sizeof(buffer)); 
	strcpy(buffer, "I get server message!");
	
	if (write(connfd, buffer, strlen(buffer)) < 0)
    {
		printf("\n Error : Write Failed \n");
		return 1;
    } 
	
	memset(buffer, '\0', sizeof(buffer)); 
	
    if (read(connfd, buffer, sizeof(buffer)) < 0)
    {
		printf("\n Error : Read Failed \n");
		return 1;
    } 
	
	printf("buffer : [%s]\n\n", buffer);
	
	close(connfd);

	return 0;
}


int main(int argc, char *argv[])
{

	socket_server(5000);

    return 0;
}


