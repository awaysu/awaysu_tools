#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LONG_LENGTH			1024

int copyfile(char *src, char *dest)
{
	int  ret = 0;
    int  nLen = 0;;
    char pBuf[MAX_LONG_LENGTH];

    int fd1 = open(src, O_RDONLY);
    int fd2 = open(dest, O_RDWR|O_CREAT, S_IRWXU);
   
    while((nLen = read(fd1, pBuf, MAX_LONG_LENGTH)) > 0)
    {
        if (write(fd2, pBuf, nLen) != nLen)
        {
            printf("%s write to file %s fail\n", __FUNCTION__, dest );	
			ret = -1;
			break;
        }
    }
          
    if (fd1) close(fd1);
    if (fd2) close(fd2);
   
    return ret;
}


int main(int argc, char *argv[])
{
    copyfile("/etc/exports", "./exports");
    return 0;
}


