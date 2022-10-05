#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid;

    /* fork a child process */
    pid = fork();

    if (pid < 0)
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0)
    { /* child process */
	printf("%s", "Child Zombie\n");
	exit(0);
    }
    else
    { /* parent process */
        sleep(10);
	wait(NULL);
        printf("%s", "Child Complete\n");
    }
    return 0;
}
