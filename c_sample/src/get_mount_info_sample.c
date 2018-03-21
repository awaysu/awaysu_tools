#include "stdio.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <mntent.h>

#define MAX_SHORT_LENGTH        128

typedef struct mountInfo
{
    char  devName[MAX_SHORT_LENGTH];
    char  mountPath[MAX_SHORT_LENGTH];
    char  fsType[MAX_SHORT_LENGTH];
	char  attribute[MAX_SHORT_LENGTH];
    struct mountInfo *next; 
} mountInfoS;

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


void getMountInfo(mountInfoS **pOutput)
{
	char *pContent = NULL;
	char *pTmp = NULL;
	char *pFeed = NULL;
	char *pLine = NULL;
	int len = 0;
	int loop = 0;
	char *sep = "\t ";
	char * pch;
	mountInfoS *pNew, *pFirst, *pCur;

	pNew = pFirst = pCur = NULL;
	
	if (load_file("/proc/mounts", &pContent) == 0)
	{
		pTmp = pContent;

		while(pFeed = strpbrk(pTmp, "\n"))
		{
			len = pFeed - pTmp;
			pLine = (char*)malloc(len + 1);
			memset(pLine, '\0', len + 1);
			strncpy(pLine, pTmp, len);
			
			pch = strtok(pLine, sep);

			if (pch)
			{
				pNew = malloc(sizeof(mountInfoS));
				memset(pNew, '\0', sizeof(mountInfoS));
				loop = 0;
				strcpy( pNew->devName, pch);
				
				while (pch != NULL)
				{
					pch = strtok (NULL, sep);
					loop++;
					
					if (loop == 1)					
						strcpy( pNew->mountPath, pch);
					else if (loop == 2)					
						strcpy( pNew->fsType, pch);	
					else if (loop == 3)					
						strcpy( pNew->attribute, pch);							
				}   

				if ( pFirst == NULL )
				{
					pFirst = pCur = pNew;
				}
				else
				{
					pCur->next = pNew;
					pCur = pNew;
				}				
			}			

			free(pLine);
			pTmp = pFeed + 1;
		}
	}

	if (pContent)
		free(pContent);
		
	*pOutput = pFirst;	
}

void freeMountInfo(mountInfoS *pIntput)
{
    mountInfoS *pCur = pIntput;
    mountInfoS *pTmp;   
    
    pTmp = pCur;
    while( pCur )
    {
        pTmp = pCur;
        pCur = pCur->next;
        free( pTmp );
    }
}


int main(int argc, char *argv[])
{
	mountInfoS *minfo = NULL;
	mountInfoS *mtmp = NULL;
	
	getMountInfo(&minfo);
	
	if(minfo)
	{
		mtmp = minfo;
		while(mtmp)
		{
			printf("devName : %s  ", mtmp->devName);
			printf("mountPath : %s  ", mtmp->mountPath);
			printf("fsType : %s  ", mtmp->fsType);
			printf("attribute : %s\n", mtmp->attribute);			
			mtmp = mtmp->next;
		}
		
		freeMountInfo(minfo);
	}
	
    return 0;
}


