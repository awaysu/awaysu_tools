#include "stdio.h"
#include <stdlib.h>
#include <string.h>

#define MAX_SHORT_LENGTH        128

typedef struct strProcMtd
{
    int                  index;
    int                  size;    
    char                 name[MAX_SHORT_LENGTH];
    char                 devname[MAX_SHORT_LENGTH];
    struct strProcMtd *next;
}strProcMtdS;

void free_mtd_info(strProcMtdS *pInArgu)
{
    strProcMtdS *pCurrent, *pNext;
	pCurrent = pInArgu;

    while(pCurrent)
    {
        pNext = pCurrent->next;
		free(pCurrent);
		pCurrent = pNext;
	}
}

void get_mtd_info(strProcMtdS  **pOutput)
{
    int idx = 0;
    FILE    *pFileFd = 0;
    size_t   readLen;    
    char *end = NULL, *start = NULL;
    int len = 0;
    char     *pLineBuf = NULL;
    char pTmp[256];
    
    strProcMtdS *pNew, *pFirst, *pCurrent;
    pNew = pFirst = pCurrent = NULL;
   
    if ( (pFileFd = fopen("/proc/mtd", "r")) != NULL )
    {
        while ( (getline(&pLineBuf, &readLen, pFileFd)) != -1)
        {               
            if (idx > 0)
            {
                pNew = malloc(sizeof(strProcMtdS));    
                memset(pNew, '0', sizeof(strProcMtdS));
           
                start = pLineBuf + strlen("mtd");
                end = strstr(pLineBuf, ":");
                if (start && end)
                {              
                    len = end - pLineBuf - strlen("mtd");
                    memset(pTmp, '\0', 256);
                    strncpy(pTmp, start, len);
                    //pNew->index = atoi(pTmp);
                    sprintf(pNew->devname, "mtd%s", pTmp);
                }
               
                start = end + 2;
                end = strstr(start, " ");
                if (start && end)
                {           
                    len = end - start;
                    memset(pTmp, '\0', 256);
                    strncpy(pTmp, start, len);
                    pNew->size = atoi(pTmp);
                }
               
                start = strstr(end, "\"") + 1;
                end = strstr(start + 1, "\"");
                if (start && end)
                {
                    len = end - start;
                    memset(pTmp, '\0', 256);
                    strncpy(pNew->name, start, len);
                }  
               
                if ( pFirst == NULL )
                {
                    pFirst = pCurrent = pNew;
                }
                else
                {
                    pCurrent->next = pNew;
                    pCurrent = pNew;
                }               
            }
            idx++;
        }
       
        *pOutput = pFirst;
       
        fclose( pFileFd ); 
    }
   
    if ( pLineBuf )
    {
        free( pLineBuf );
    }
}


int main(int argc, char *argv[])
{
	strProcMtdS *mtdinfo = NULL;
	strProcMtdS *pTmp = NULL;
	
	get_mtd_info(&mtdinfo);
	
	if (mtdinfo)
	{	
		pTmp = mtdinfo;
		
		while(pTmp)
		{
			printf("index : %d, name : %s, devname : %s, size : %d\n"
				, pTmp->index, pTmp->name, pTmp->devname, pTmp->size);
				
			pTmp = pTmp->next;
		}
			
		free_mtd_info(mtdinfo);
	}

    return 0;
}


