#include "stdio.h"
#include <stdlib.h>
#include "sample.h"


BOOL check_program(char *exe_name)
{
    int bRet = FALSE;
    char command[MAX_SHORT_LENGTH];
    
    sprintf(command, "ps | grep %s | grep -v grep > /dev/null", exe_name);
    if (system(command) == 0)
        bRet = TRUE;
        
    return bRet; 
}


#ifndef MAKE_LIBRARY_SAMPLE
int main(int argc, char *argv[])
{
	BOOL ret = check_program("bash");
	printf("bash is %s\n", ret==TRUE?"running!":"not running!");	
	
    return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */

