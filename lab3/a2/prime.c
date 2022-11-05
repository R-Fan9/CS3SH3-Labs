
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 1

typedef struct
{
    int num;
} parameters;

int is_prime(int num)
{

    for (int i = 2; i < num; i++)
    {
        if (num % i == 0)
        {
            return 0;
        }
    }

    return 1;
}

void *get_primes(void *params)
{
    parameters *data = (parameters *)params;

    for (int i = 2; i <= data->num; i++)
    {
        if (is_prime(i) == 1)
        {
            printf("%d\n", i);
        }
    }

    pthread_exit(0);
}

int main(int argc, const char *argv[])
{
    pthread_t workers[NUMBER_OF_THREADS];

    int num = strtol(argv[1], 0, 10);

    parameters *data = (parameters *)malloc(sizeof(parameters));
    data->num = num;
    pthread_create(&workers[0], 0, get_primes, data);

    pthread_join(workers[0], 0);

    return 0;
}