#include "stdio.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "sample.h"

#ifndef MAKE_LIBRARY_SAMPLE
BOOL load_file(char *pFile, char **pContent)
{
    int nTotalLen = 0, readLen;
    int fd;
    char pBuf[0x100];
    
    if ( (fd = open((char *)pFile, O_RDONLY)) < 0 )
    {
        printf("Open File %s FALSE %d\n", pFile, strlen((char *)pFile));
        return FALSE;
    }

    nTotalLen = 0;
    while( (readLen = read(fd, pBuf, 0x100 )) > 0)
        nTotalLen += readLen;

    if (nTotalLen <= 0)
    {
        printf("%s the file length is %d\n", __FUNCTION__, nTotalLen);
        return FALSE;
    }        

    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        printf("%s can't seek file\n", __FUNCTION__);
        return FALSE;
    }

    // alloc memory
    *pContent = (char*)malloc( ( nTotalLen + 1)* sizeof(char));

    if(read(fd, *pContent, nTotalLen) < 0)
    {
        free(*pContent);
        return FALSE;
    }
    
    *(*pContent + nTotalLen ) = '\0';
    
    close(fd);
    return TRUE;    
}
#endif /* MAKE_LIBRARY_SAMPLE */

void load_line(char *file)
{
	char *pContent = NULL;
	char *pTmp = NULL;
	char *pFeed = NULL;
	char *pLine = NULL;
	int len = 0;

	if (load_file(file, &pContent) == TRUE)
	{
	  pTmp = pContent;

	  while(pFeed = strpbrk(pTmp, "\n"))
	  {
		   len = pFeed - pTmp;
		   pLine = (char*)malloc(len + 1);
		   memset(pLine, '\0', len + 1);
		   strncpy(pLine, pTmp, len);
		   printf("pLine : %s\n", pLine);
		   free(pLine);
		   pTmp = pFeed + 1;
	  }

	}

	if (pContent)
	 free(pContent);
}

#ifndef MAKE_LIBRARY_SAMPLE
int main(int argc, char *argv[])
{

	load_line("/etc/exports");
	
    return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */

