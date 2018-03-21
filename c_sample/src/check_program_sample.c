#include "stdio.h"
#include <stdlib.h>

#define MAX_SHORT_LENGTH        128

int check_program(char *exe_name)
{
    int bRet = -1;
    char command[MAX_SHORT_LENGTH];
    
    sprintf(command, "ps | grep %s | grep -v grep > /dev/null", exe_name);
    if (system(command) == 0)
        bRet = 0;
        
    return bRet; 
}



int main(int argc, char *argv[])
{
	int ret = check_program("bash");
	printf("bash is %s\n", ret==0?"running!":"not running!");	
	
    return 0;
}


