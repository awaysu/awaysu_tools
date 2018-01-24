#include "stdio.h"
#include "sample.h"

#ifdef _DEBUG
int (*printf_d)(const char*, ...) = printf;
#else
int printf_d(const char* fmt, ...) {return 0;}
#endif


#ifndef MAKE_LIBRARY_SAMPLE
int main(int argc, char *argv[])
{
	printf("%s %d\n", __FUNCTION__, __LINE__);
    return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */

