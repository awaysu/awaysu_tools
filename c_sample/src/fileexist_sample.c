#include "stdio.h"
#include <unistd.h>
#include "sample.h"

BOOL is_file_exist(char *pFile)
{
	int bRet = FALSE;
	FILE* fp = fopen(pFile, "r");
	if (fp) 
	{
	    bRet = TRUE;
	    fclose(fp);
	}
	return bRet;
}

BOOL is_folder_exist(char *pFolder)
{
	BOOL bRet = FALSE;
    int ret = chdir(pFolder);
	if (ret == 0)
		bRet = TRUE;
	return bRet;
}

#ifndef MAKE_LIBRARY_SAMPLE
int main(int argc, char *argv[])
{
	BOOL ret = is_file_exist("/etc/exports");
    printf("/etc/exports : %s\n", ret==TRUE?"Exist":"Not Exist");
	
	ret = is_folder_exist("/etc/");
    printf("/etc/ : %s\n", ret==TRUE?"Exist":"Not Exist");	
    return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */
