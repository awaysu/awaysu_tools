#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern int errno;

#ifndef MAKE_LIBRARY_SAMPLE
int main(int argc, char *argv[])
{
    strtol("0xfffffffff",NULL,0);
    char *err_str = strerror(errno);
    printf("error : %d, %s\n", errno, err_str);
    return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */

