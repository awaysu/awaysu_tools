#include "stdio.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


	   
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


int main(int argc, char *argv[])
{
    char save[] = "abcde\n12345\n";
	
	save_file("./save.txt", save, strlen(save));
	
    return 0;
}
