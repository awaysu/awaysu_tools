#include "stdio.h"
#include <sys/sysinfo.h>



int main(int argc, char *argv[])
{
	struct sysinfo sys_info;
	
    if ( sysinfo( &sys_info ) == 0 )
    {
		printf("Total Ram: %lluM\tFree: %lluM\n",
			sys_info.totalram *(unsigned long long)sys_info.mem_unit/1024/1024,
			sys_info.freeram *(unsigned long long)sys_info.mem_unit/1024/1024);
    }
    return 0;
}


