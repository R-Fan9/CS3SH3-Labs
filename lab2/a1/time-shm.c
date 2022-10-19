#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{

    char **ptr = argv;

    while (*ptr != NULL)
    {
        printf("%s\n", *ptr);
        ptr++;
    }
    const char *programName = "ls";
    const char *args[] = {programName, ".", NULL};
    execvp(programName, args);

    // /* the size (in bytes) of shared memory object */
    // const int SIZE = 4096;
    // /* name of the shared memory object */
    // const char *name = "OS";
    // /* shared memory file descriptor */
    // int fd;
    // /* pointer to shared memory obect */
    // char *ptr;
    // struct timeval current_time;

    // /* create the shared memory object */
    // fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    // /* configure the size of the shared memory object */
    // ftruncate(fd, SIZE);
    // /* memory map the shared memory object */
    // ptr = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // pid_t pid;

    // pid = fork();

    // if (pid == 0)
    // {
    //     gettimeofday(&current_time, NULL);
    //     sprintf(ptr, "%ld", current_time.tv_sec);
    //     execvp(argv[0], argv);
    // }
    // else if (pid > 0)
    // {
    //     wait(NULL);
    //     /* open the shared memory object */
    //     fd = shm_open(name, O_RDONLY, 0666);
    //     /* memory map the shared memory object */
    //     ptr = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    //     /* read from the shared memory object */
    //     // printf("%s", (char *)ptr);
    //     char *start_time = (char *)ptr;

    //     gettimeofday(&current_time, NULL);

    //     printf("Elasped time: %ld", current_time.tv_sec - start_time);

    //     /* remove the shared memory object */
    //     shm_unlink(name);
    // }

    return 0;
}