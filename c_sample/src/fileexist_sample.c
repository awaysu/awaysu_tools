#include "stdio.h"
#include <unistd.h>


int is_file_exist(char *pFile)
{
	int bRet = -1;
	FILE* fp = fopen(pFile, "r");
	if (fp) 
	{
	    bRet = 0;
	    fclose(fp);
	}
	return bRet;
}

int is_folder_exist(char *pFolder)
{
	int bRet = -1;
    int ret = chdir(pFolder);
	if (ret == 0)
		bRet = 0;
	return bRet;
}


int main(int argc, char *argv[])
{
	int ret = is_file_exist("/etc/exports");
    printf("/etc/exports : %s\n", ret==0?"Exist":"Not Exist");
	
	ret = is_folder_exist("/etc/");
    printf("/etc/ : %s\n", ret==0?"Exist":"Not Exist");	
    return 0;
}

