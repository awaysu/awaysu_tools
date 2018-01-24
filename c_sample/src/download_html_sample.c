#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define USERAGENT "HTMLGET 1.0"
 
int create_tcp_socket()
{
    int sock;
    if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("Can't create TCP socket");
        exit(1);
    }
    return sock;
}
 
char *get_ip(char *host)
{
    struct hostent *hent;
    int iplen = 15; //XXX.XXX.XXX.XXX
    char *ip = (char *)malloc(iplen+1);
    memset(ip, 0, iplen+1);
    if((hent = gethostbyname(host)) == NULL)
    {
        herror("Can't get IP");
        exit(1);
    }
    if(inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL)
    {
        perror("Can't resolve host");
        exit(1);
    }
    return ip;
}
 
char *build_get_query(char *host, char *page)
{
    char *query;
    char *getpage = page;
    char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
    if(getpage[0] == '/'){
        getpage = getpage + 1;
        fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
    }
    // -5 is to consider the %s %s %s in tpl and the ending \0
    query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);
    sprintf(query, tpl, getpage, host, USERAGENT);
    return query;
}

int get_http_code(char *host, char *page, int port, char **code)
{
    struct sockaddr_in *remote;
    int sock;
    int tmpres;
    char *ip;
    char *get;
    char buf[BUFSIZ+1];
 
    sock = create_tcp_socket();
    ip = get_ip(host);
    //fprintf(stderr, "IP is %s\n", ip);
    remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
    remote->sin_family = AF_INET;
    tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
    if( tmpres < 0) 
    {
        perror("Can't set remote->sin_addr.s_addr");
        return -1;
    }else if(tmpres == 0)
    {
        fprintf(stderr, "%s is not a valid IP address\n", ip);
        return -1;
    }
    remote->sin_port = htons(port);
 
    if(connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0)
    {
        perror("Could not connect");
        return -1;
    }
 
    get = build_get_query(host, page);
    //fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);
 
    //Send the query to the server
    int sent = 0;
    while(sent < strlen(get))
    {
        tmpres = send(sock, get+sent, strlen(get)-sent, 0);
        if(tmpres == -1){
            perror("Can't send query");
            exit(1);
        }
        sent += tmpres;
    }
    //now it is time to receive the page
    memset(buf, 0, sizeof(buf));
    int htmlstart = 0;
    char * htmlcontent;
    while((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0)
    {
        if(htmlstart == 0)
        {
            /* Under certain conditions this will not work.
            * If the \r\n\r\n part is splitted into two messages
            * it will fail to detect the beginning of HTML content
            */
            htmlcontent = strstr(buf, "\r\n\r\n");
            if(htmlcontent != NULL){
                htmlstart = 1;
                htmlcontent += 4;
            }
        }else{
            htmlcontent = buf;
        }
        if(htmlstart)
        {
            char *pTmp = malloc(strlen(htmlcontent) + 1);
            memset(pTmp, '\0', strlen(htmlcontent) + 1);
            strcpy(pTmp, htmlcontent);
            *code = pTmp;
            //fprintf(stdout, htmlcontent);
        }
 
        memset(buf, 0, tmpres);
    }
 
    if(tmpres < 0)
    {
        perror("Error receiving data");
    }
    free(get);
    free(remote);
    free(ip);
    close(sock);
 
    return 0;
}

#ifndef MAKE_LIBRARY_SAMPLE
int main(int argc, char *argv[])
{
	char *pTmp = NULL;
	get_http_code("people.cs.nctu.edu.tw", "/~yslin/library/linuxc/main.htm", 80, &pTmp);
 
	if (pTmp)
	{
		printf("HTML Code : [%s]\n", pTmp);
		free(pTmp);
	}

	return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */

