#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

unsigned int get_process_id(char *app_name)
{
    DIR *dir_p;
    struct dirent *dir_entry_p;
    char dir_name[40];                                        // ??? buffer overrun potential
    char target_name[252];                                    // ??? buffer overrun potential
    int target_result;
    char exe_link[252];
    //int errorcount = 0;
    int result = -1;

    dir_p = opendir("/proc/");                                                                 // Open /proc/ directory
    while(NULL != (dir_entry_p = readdir(dir_p))) {                                            // Reading /proc/ entries
        if (strspn(dir_entry_p->d_name, "0123456789") == strlen(dir_entry_p->d_name)) {        // Checking for numbered directories
            strcpy(dir_name, "/proc/");
            strcat(dir_name, dir_entry_p->d_name);
            strcat(dir_name, "/");                                                             // Obtaining the full-path eg: /proc/24657/
            exe_link[0] = 0;
            strcat(exe_link, dir_name);
            strcat(exe_link, "exe");                                                         // Getting the full-path of that exe link
            target_result = readlink(exe_link, target_name, sizeof(target_name)-1);            // Getting the target of the exe ie to which binary it points to
            if (target_result > 0) {
                target_name[target_result] = 0;
                if (strstr(target_name, app_name) != NULL) {                            // Searching for process name in the target name -- ??? could be a better search !!!
                    result = atoi(dir_entry_p->d_name);
                    //printf("getProcessID(%s) :Found. id = %d\n", app_name, result);
                    closedir(dir_p);
                    return result;
                }
            }
        }
    }
    closedir(dir_p);
    return result;
}

#ifndef MAKE_LIBRARY_SAMPLE
int main(int argc, char *argv[])
{
	printf("smbd : id %d\n", get_process_id("smbd"));
    return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */

