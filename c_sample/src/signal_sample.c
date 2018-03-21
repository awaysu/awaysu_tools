#include "stdio.h"
#include <unistd.h>
#include <signal.h>


static int loop = 1;

void alarm_handler(int a)
{
    printf("Alarm!!!\n");
	loop = 0;
}


int main(int argc, char *argv[])
{
    signal(SIGALRM, alarm_handler);
    alarm(3);
	
	while(loop){}
	
	printf("Program exit!!!\n");
	
    return 0;
}


