#ifndef SAMPLE_INC_H
#define SAMPLE_INC_H

#define BOOL					int
#define TRUE					0
#define FALSE					-1
#define MAX_LONG_LENGTH			1024
#define MAX_SHORT_LENGTH        128

/* ============================================================= */
typedef struct mountInfo
{
    char  devName[MAX_SHORT_LENGTH];
    char  mountPath[MAX_SHORT_LENGTH];
    char  fsType[MAX_SHORT_LENGTH];
	char  attribute[MAX_SHORT_LENGTH];
    struct mountInfo *next; 
} mountInfoS;
/* ============================================================= */
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
/* ============================================================= */
typedef struct ConfigParam
{
    char   argument[MAX_SHORT_LENGTH];
    char   option[MAX_SHORT_LENGTH];
    struct ConfigParam *next; 
}ConfigParamS;
/* ============================================================= */

typedef struct strProcMtd
{
    int                  index;
    int                  size;    
    char                 name[MAX_SHORT_LENGTH];
    char                 devname[MAX_SHORT_LENGTH];
    struct strProcMtd *next;
}strProcMtdS;
/* ============================================================= */

extern void get_time(char *pTime);
extern BOOL set_time(char *buffertime); 
extern int strtonum(char *pInput);
extern int socket_server(int port);
extern int socket_client(char *ip, int port);
extern BOOL save_file(char *pFile, char *pBuf, unsigned int length);
extern void process_options(int argc, char *argv[]);
extern int get_mac_address(char *name, char *mac);
extern int get_ip_address(char *name, char *ip);
extern int get_netlink_status(const char *if_name);
extern void get_mtd_info(strProcMtdS  **pOutput);
extern void free_mtd_info(strProcMtdS *pInArgu);
extern void load_line(char *file);
extern BOOL load_file(char *pFile, char **pContent);
extern int get_host_ip(char *host, char *ip);
extern unsigned int get_process_id(char *app_name);
extern void getMountInfo(mountInfoS **pOutput);
extern void freeMountInfo(mountInfoS *pIntput);
extern int get_file_list(char *path, filefolderInfoS **arcfolderlist);
extern void free_file_list(filefolderInfoS *folderlist);
extern BOOL is_file_exist(char *pFile);
extern BOOL is_folder_exist(char *pFolder);
extern int get_http_code(char *host, char *page, int port, char **code);
extern BOOL copyfile(char *src, char *dest);
extern BOOL set_config_value(char *file, char *pArgument, char *pOption);
extern int get_config_value(char *file, char *pArgument, char *pOption);
extern BOOL check_program(char *exe_name);

#endif /* SAMPLE_INC_H */

