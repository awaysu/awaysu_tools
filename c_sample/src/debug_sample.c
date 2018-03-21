#include "stdio.h"


#ifdef _DEBUG
int (*printf_d)(const char*, ...) = printf;
#else
int printf_d(const char* fmt, ...) {return 0;}
#endif



int main(int argc, char *argv[])
{
	printf("%s %d\n", __FUNCTION__, __LINE__);
    return 0;
}


