#include "stdio.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>



int load_file(char *pFile, char **pContent)
{
    int nTotalLen = 0, readLen;
    int fd;
    char pBuf[0x100];
    
    if ( (fd = open((char *)pFile, O_RDONLY)) < 0 )
    {
        printf("Open File %s -1 %d\n", pFile, (int)strlen((char *)pFile));
        return -1;
    }

    nTotalLen = 0;
    while( (readLen = read(fd, pBuf, 0x100 )) > 0)
        nTotalLen += readLen;

    if (nTotalLen <= 0)
    {
        printf("%s the file length is %d\n", __FUNCTION__, nTotalLen);
        return -1;
    }        

    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        printf("%s can't seek file\n", __FUNCTION__);
        return -1;
    }

    // alloc memory
    *pContent = (char*)malloc( ( nTotalLen + 1)* sizeof(char));

    if(read(fd, *pContent, nTotalLen) < 0)
    {
        free(*pContent);
        return -1;
    }
    
    *(*pContent + nTotalLen ) = '\0';
    
    close(fd);
    return 0;    
}


int main(int argc, char *argv[])
{
    char *pContent = NULL;
	
	if (load_file("/etc/exports", &pContent) == 0)
	{
		printf("%s\n", pContent);
		
		if (pContent)
			free(pContent);
	}
	
    return 0;
}

