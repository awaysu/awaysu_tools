#include "stdio.h"


int main(int argc, char *argv[])
{
	filefolderInfoS *pInfo = NULL;
	filefolderInfoS *pTmp = NULL;
	
	get_file_list("/etc/", &pInfo);
	
	pTmp = pInfo;
	
	if (pTmp)
	{
		while(pTmp)
		{
			printf("file : %s, %s, %d\n", pTmp->pathname, pTmp->name, pTmp->type);
			pTmp = pTmp->next;
		}
		
		free_file_list(pInfo);
	}
	
    return 0;
}

