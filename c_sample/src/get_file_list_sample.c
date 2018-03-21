#include <stdio.h>
#include <stdlib.h>
#include <linux/usbdevice_fs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <scsi/scsi.h>
#include <scsi/sg.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <dirent.h>
#include <string.h>

#define MAX_SHORT_LENGTH        128

typedef enum FILEFOLDER_TYPE
{
    ATTRIBUTE_FILE,
    ATTRIBUTE_FOLDER,
}FILEFOLDER_TYPE_ENUM;

typedef struct filefolderInfo
{
    char                        pathname[MAX_SHORT_LENGTH*2];
    char                        name[MAX_SHORT_LENGTH*2];
    FILEFOLDER_TYPE_ENUM   	    type;
    struct filefolderInfo *next;
}filefolderInfoS;

int get_file_list(char *path, filefolderInfoS **arcfolderlist)
{
    DIR *dir;
    struct dirent *ptr;
    filefolderInfoS *first = NULL;
    filefolderInfoS *now = NULL;

    dir =opendir(path);
    if (dir == NULL)
        return 0;

    while((ptr = readdir(dir))!=NULL) 
    {
        if ( strcmp(ptr->d_name, ".") == 0 
          || strcmp(ptr->d_name, "lost+found") == 0
          || strcmp(ptr->d_name, "..") == 0)
        {
            continue;
        }
        
        if (strlen(ptr->d_name) > 1 && ptr->d_name[0] == '.')
            continue;
        
        char pathname[0x200];
        memset(pathname, '\0', 0x200);
        struct stat buf;
        sprintf(pathname,"%s%s", path, ptr->d_name);
        stat(pathname, &buf);
        
        filefolderInfoS *list = malloc(sizeof(filefolderInfoS));
        strcpy(list->pathname, pathname);
        strcpy(list->name, ptr->d_name);     
        list->next = NULL;
        
        if(S_ISREG(buf.st_mode))
            list->type = ATTRIBUTE_FILE;
        else
            list->type = ATTRIBUTE_FOLDER;
        
        //printf("%s, %s, %d\n", list->pathname, list->name, list->type);
        
        if (now == NULL)
        {
            first = list;
            now = list;
        }
        else
        {
            now->next = (filefolderInfoS*)list;
            now = list;
        }
            
    }
    closedir(dir);
    
    *arcfolderlist = first;
    
    return 0;

}

void free_file_list(filefolderInfoS *folderlist)
{
    filefolderInfoS *pCurrent, *pNext;
	pCurrent = folderlist;

    while(pCurrent)
    {
        pNext = (filefolderInfoS*)pCurrent->next;
		free(pCurrent);
		pCurrent = pNext;
	}
}


void easy_sample()
{
    DIR * dir;
    struct dirent * ptr;
    dir =opendir("./");
   
    while((ptr = readdir(dir))!=NULL)
    {
        printf("d_name: %s\n", ptr->d_name);
    }
   
    closedir(dir);
}



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


