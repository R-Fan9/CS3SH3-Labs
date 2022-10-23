/**
 * time-pipe.c
 *
 * C program that calculates 
 * the execution time of commands  
 * using pipe 
 *
 * fanh11@mcmaster.ca
 */

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

// define the index for the read end of the pipe 
#define READ_END 0
// define the index for the write end of the pipe 
#define WRITE_END 1

int main(int argc, char *argv[])
{
    /**
     * define an array to store the file descriptor 
     * for the read & write end of the pipe
     */
    int fd[2];
    // process Id
    pid_t pid;
    // variable to keep track of the current time 
    struct timeval current_time;

    struct timeval *ptr = &current_time;
    
	/* create the pipe */
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return -1;
    }

	/* fork a child process */
    pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Fork failed");
        return -1;
    }
    /* child process */
    else if (pid == 0)
    {
        /* close the read end of the pipe when write is about to happen*/
        close(fd[READ_END]);
        
	    /* get a record of the current time before executing the input command */
        gettimeofday(&current_time, NULL);

	    /* write the address of the current time to the pipe */
        write(fd[WRITE_END], ptr, sizeof(struct timeval));

	    /* close the write end of the pipe*/
        close(fd[WRITE_END]);

        execvp(argv[1], &argv[1]);
    }
    /* parent process */
    else
    {
        /* wait for child process to complete */
        wait(NULL);

        /* close the write end of the pipe when read is about to happen*/
        close(fd[WRITE_END]);

	    /* read the address of the start time record from the pipe */
        read(fd[READ_END], ptr, sizeof(struct timeval));
        
	    /* calculate the start time by converting to seconds */
        double st_sec = ptr->tv_sec+ptr->tv_usec/1000000.0;

	    /* get the current time after the input command has completed in the child process*/
        gettimeofday(&current_time, NULL);

	    /* calculate the current time by converting to seconds */
        double ct_sec = current_time.tv_sec+current_time.tv_usec/1000000.0;

	    /* display the elasped time for executing the input command */
        printf("\nElapsed time: %lf seconds\n", ct_sec - st_sec);

	    /* close the read end of the pipe*/
        close(fd[READ_END]);
    }

    return 0;
}
