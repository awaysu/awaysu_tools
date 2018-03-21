#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/* fork()����Thread,�L�|���ͤ��i�{�M�l�i�{�����G�Y����,�N���O�G��Thread. (���M�ݦ��P�ɶ��i��,���ƹ�W�٬O���@�ӥ����@�ӫ�.) */
int fork_sample1()
{
    pid_t child_pid;
    printf ("The main program process ID is %d\n", (int) getpid ());
    child_pid = fork ();

    if (child_pid != 0)
    {
         // �l�i�{
        printf ("This is the parent process, with id %d\n", (int) getpid ());
        printf ("he child process ID is %d\n", (int) child_pid);
    }
    else
    {
        // ���i�{
        printf ("his is the child process, with id %d\n", (int) getpid ());
        printf ("his is the parent process, with id %d\n", (int) getppid ());
    }

    return 0;
}

/* ��Y�l�i�{���פ��(��{���̫�return 0), �Ӥ��i�{�٥��פ��,�|�Ӧ��{�������`����,�ҥH�����f�twait�Өϥ�. */
int fork_sample2()
{
    pid_t child_pid;
    int child_status;

    printf ("The main program process ID is %d\n", (int) getpid ());
    child_pid = fork ();

    if (child_pid != 0)
    {
        printf ("This is the parent process, with id %d\n", (int) getpid ());
        printf ("he child�s process ID is %d\n", (int) child_pid);

        /* Wait for the child process to complete. */
        wait (&child_status);
        if (WIFEXITED (child_status))
        {
            printf ("he child process exited normally, with exit code %d\n", WEXITSTATUS (child_status));
        }
       else
        {
           printf ("he child process exited abnormally\n");
        }
    }
    else
    {
        printf ("his is the child process, with id %d\n", (int) getpid ());
        printf ("his is the parent process, with id %d\n", (int) getppid ());
        sleep(5);
    }

    return 0;
}


int main(int argc, char *argv[])
{
	printf("fork_sample1\n");
    fork_sample1();
	printf("\n\nfork_sample2\n");
	fork_sample2();
	
    return 0;
}
