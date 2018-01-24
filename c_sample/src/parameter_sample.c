#include <stdio.h>
#include <unistd.h>

#ifndef MAKE_LIBRARY_SAMPLE

void process_options(int argc, char *argv[])
{
    int oc; 
    char *b_opt_arg;

    while((oc = getopt(argc, argv, "ngl:")) != -1)
    {
        switch(oc)
        {
            case 'n':
                printf("My name is Lyong.\n");
                break;
             case 'g':
                printf("Her name is Xxiong.\n");
                break;
            case 'l':
                b_opt_arg = optarg;
                printf("Our love is %s\n", optarg);
                break;
            default:
                printf("arguments error!\n");
                break;
        }
    }
}

int main(int argc, char *argv[])
{
	process_options(argc, argv);
	
    return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */

