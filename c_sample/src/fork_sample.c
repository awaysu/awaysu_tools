#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/* fork()類似Thread,他會產生父進程和子進程分成二頭執行,就像是二條Thread. (雖然看似同時間進行,但事實上還是有一個先有一個後.) */
int fork_sample1()
{
    pid_t child_pid;
    printf ("The main program process ID is %d\n", (int) getpid ());
    child_pid = fork ();

    if (child_pid != 0)
    {
         // 子進程
        printf ("This is the parent process, with id %d\n", (int) getpid ());
        printf ("he child process ID is %d\n", (int) child_pid);
    }
    else
    {
        // 父進程
        printf ("his is the child process, with id %d\n", (int) getpid ());
        printf ("his is the parent process, with id %d\n", (int) getppid ());
    }

    return 0;
}

/* 當某子進程先終止時(到程式最後return 0), 而父進程還未終止時,會照成程式不正常關閉,所以必須搭配wait來使用. */
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
