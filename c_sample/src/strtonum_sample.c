#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include "sample.h"

int strtonum(char *pInput)
{
    char *pTmp;
    if ( *pInput == '0' )
    {
        if ( *(pInput + 1) == 'x' || 
        	     *(pInput + 1) == 'X' )
        {
            pTmp = pInput + 2;
            return strtoll( pTmp, NULL, 16 );
        }
    }
    
    return strtoll( pInput, NULL, 10 );
}


#ifndef MAKE_LIBRARY_SAMPLE
int main(int argc, char *argv[])
{
	char val[] = "0x30";
	int ret = strtonum(val);
	printf("%s = 0x%x\n", val, ret);
	
    return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */

