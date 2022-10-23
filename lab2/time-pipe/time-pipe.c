#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    int fd[2];
    pid_t pid;
    struct timeval current_time;

    struct timeval *ptr = &current_time;
    

    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return -1;
    }

    pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Fork failed");
        return -1;
    }
    else if (pid == 0)
    {
        close(fd[READ_END]);
        gettimeofday(&current_time, NULL);

        write(fd[WRITE_END], ptr, sizeof(struct timeval));

        close(fd[WRITE_END]);

        execvp(argv[1], &argv[1]);
    }
    else
    {
        wait(NULL);

        close(fd[WRITE_END]);

        read(fd[READ_END], ptr, sizeof(struct timeval));
        
        double st_sec, ct_sec;  

        st_sec = ptr->tv_sec+ptr->tv_usec/1000000.0;

        gettimeofday(&current_time, NULL);
        ct_sec = current_time.tv_sec+current_time.tv_usec/1000000.0;

        printf("\nElapsed time: %lf seconds\n", ct_sec - st_sec);

        close(fd[READ_END]);
    }

    return 0;
}
