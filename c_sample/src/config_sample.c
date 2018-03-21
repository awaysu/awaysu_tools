#include "stdio.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define MAX_SHORT_LENGTH        128

typedef struct ConfigParam
{
    char   argument[MAX_SHORT_LENGTH];
    char   option[MAX_SHORT_LENGTH];
    struct ConfigParam *next; 
}ConfigParamS;

int load_file(char *pFile, char **pContent)
{
    int nTotalLen = 0, readLen;
    int fd;
    char pBuf[0x100];
    
    if ( (fd = open((char *)pFile, O_RDONLY)) < 0 )
    {
        printf("Open File %s -1 %d\n", pFile, fd);
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

int save_file(char *pFile, char *pBuf, unsigned int length)
{
    int    fd = 0;
    int    retValue = -1;
	
    if ( (fd = open( pFile, 
    	      O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) > 0 )
    {
		if ( write(fd, pBuf, length) == length )
			retValue = 0;
		else		
			printf("%s write to file %s fail\n", __FUNCTION__, pFile );	
			
		if (fd)	close(fd);
		sync();			
	}
	else
        printf("%s create %s file fail\n", __FUNCTION__, pFile);
    
    return retValue;
}


void make_param_default_value(ConfigParamS **pArgu)
{
    ConfigParamS *pNew;
	pNew = malloc(sizeof(ConfigParamS));
	memset(pNew->argument, '\0', sizeof(pNew->argument));
	memset(pNew->option, '\0', sizeof(pNew->option));
    pNew->next = NULL;
    *pArgu = pNew;
}

void get_param_option(char *pInArgu, ConfigParamS **pOutArgu)
{
    int nindex, nstart, nend;
    nindex = nstart = nend = 0;
    ConfigParamS *pFirst, *pCurrent, *pNew;
    pFirst = pCurrent = pNew = NULL;
    void *pBuf = NULL; 
    pBuf = malloc((strlen(pInArgu) + 1) * sizeof(char));
    char* pbuff = NULL;
    pbuff = (char *)pBuf;
    strcpy(pbuff, pInArgu);

    while (nindex < strlen(pInArgu))
    {
        if (*(pInArgu + nindex) == '=')
        {
            make_param_default_value(&pNew);
            if (pFirst == NULL)
            {
                pFirst = pCurrent = pNew;
            } else {
                pCurrent->next = pNew;
                pCurrent       = pNew;
            }
            
            nend = nindex;
            memcpy(pCurrent->argument, pbuff + nstart, nend - nstart);
            nstart = nindex + 1;
       }
       else if ( *(pInArgu + nindex) == '\n' || *(pInArgu + nindex + 1 ) == '\0')
       {
           nend = nindex;
           memcpy(pCurrent->option, pbuff + nstart, nend - nstart);
           //deletespace(pCurrent->option);
           nstart = nend + 1;
       }
       nindex ++;
    }
    *pOutArgu = pFirst;
}

void free_param_option(ConfigParamS *pInArgu)
{
    ConfigParamS *pCurrent, *pNext;
	pCurrent = pInArgu;

    while(pCurrent)
    {
        pNext = pCurrent->next;
		free(pCurrent);
		pCurrent = pNext;
	}
}

int set_config_value(char *file, char *pArgument, char *pOption)
{
    char pWrite[2048];
    int ret = -1;
    char *pBuf = NULL;
    ConfigParamS *pOutArgu = NULL;
    ConfigParamS *pTmp = NULL;
    ConfigParamS *pTmp2 = NULL;
    
    memset(pWrite, '\0', 2048);
    load_file(file, &pBuf);
    
    if (pBuf)
    {
        get_param_option(pBuf, &pOutArgu);
        if (pOutArgu)
        {
            pTmp = pOutArgu;
            
            while(pTmp)
            {
                if (strcmp(pTmp->argument, pArgument) ==0)
                {
                    ret = 0;
                    strcpy(pTmp->option, pOption);
                    break;
                }
                pTmp2 = pTmp;
                pTmp = pTmp->next;
            }
            
            if (ret == -1)
            {
                ConfigParamS *pNew = malloc(sizeof(ConfigParamS));
                memset(pNew->argument, '\0', sizeof(pNew->argument));
                memset(pNew->option, '\0', sizeof(pNew->option));
                strcpy(pNew->argument, pArgument);
                strcpy(pNew->option, pOption);
                pNew->next = NULL;
                pTmp2->next = pNew;
            }
            
            pTmp = pOutArgu;
            
            while(pTmp)
            {
                strcat(pWrite, pTmp->argument);
                strcat(pWrite, "=");
                strcat(pWrite, pTmp->option);
                strcat(pWrite, "\n");
                pTmp = pTmp->next;
            }
            
            save_file(file, pWrite, strlen(pWrite));
            
            free_param_option(pOutArgu);            
        }
		
		free(pBuf);
    }  
    else
    {
        sprintf(pWrite, "%s=%s\n", pArgument, pOption);
        save_file(file, pWrite, strlen(pWrite));
    }
    
    return 0;
    
}

int get_config_value(char *file, char *pArgument, char *pOption)
{
    int ret = -1;
    char *pBuf = NULL;
    ConfigParamS *pOutArgu = NULL;
    ConfigParamS *pTmp = NULL;
    
    load_file(file, &pBuf);
    
    if (pBuf)
    {
        get_param_option(pBuf, &pOutArgu);
        if (pOutArgu)
        {
            pTmp = pOutArgu;
            
            while(pTmp)
            {
                if (strcmp(pTmp->argument, pArgument) ==0)
                {
                    ret = 0;
                    strcpy(pOption, pTmp->option);
                    break;
                }
                pTmp = pTmp->next;
            }
            free_param_option(pOutArgu);            
        }
		
		free(pBuf);
    }       
    
    return ret;
}



int main(int argc, char *argv[])
{
	char value[MAX_SHORT_LENGTH];
	
	set_config_value("./test.conf", "abc", "123");
	set_config_value("./test.conf", "efg", "456");
	get_config_value("./test.conf", "abc", value);
	
	printf("abc : %s\n", value);
	
    return 0;
}


