#define _GNU_SOURCE 
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <unistd.h> 
#include <sched.h>
#include <pthread.h>

int main(int argc, char *argv[])
{
	int cpuid = 0;
	cpu_set_t mask;

	if (argv[1])
		cpuid = atoi(argv[1]);

	printf("cpu id : %i\n", cpuid);	
	
	CPU_ZERO(&mask);
	CPU_SET(cpuid, &mask);

	if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
	{
		printf("warning: could not set CPU affinity, continuing...\n");
	}
	
	printf("You can type \"busybox mpstat -P ALL 2 3\" to check CPU status.\n");
    while(1)
    {
	
    }

    return 0;
}





