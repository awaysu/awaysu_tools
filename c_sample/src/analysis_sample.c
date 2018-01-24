#include "stdio.h"
#include "sample.h"

#ifndef MAKE_LIBRARY_SAMPLE
int main(int argc, char *argv[])
{
	int g_width, g_height;
	
	if (sscanf("88x99", "%dx%d", &g_width, &g_height) != 2)
		printf("sscanf fail!\n");
	else
		printf("%d, %d\n", g_width, g_height);
	
    return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */

