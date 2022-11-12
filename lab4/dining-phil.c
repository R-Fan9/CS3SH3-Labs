#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER_OF_THREADS 5
#define CYCLES 2

typedef struct
{
    int phil_num;
} parameters;

int completed_phils[] = {0, 0, 0, 0, 0};
int available_forks[] = {1, 1, 1, 1, 1};

pthread_mutex_t mutex;
pthread_cond_t cond_t;

void think(int philosopher_number)
{
    int r = rand() % 3 + 1;

    printf("phil_%d is thinking for %d secs | ", philosopher_number, r);
    sleep(r);
}

void eat(int philosopher_number)
{
    int r = rand() % 3 + 1;

    printf("phil_%d is eating for %d secs | ", philosopher_number, r);
    sleep(r);
}

void pickup_forks(int philosopher_number)
{
    int fork_i = philosopher_number;
    int fork_j = (fork_i + 1) % NUMBER_OF_THREADS;

    pthread_mutex_lock(&mutex);

    printf("\n");
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        printf("fork_%d status: %d\n", i, available_forks[i]);
    }
    printf("\n");

    while (!available_forks[fork_i] || !available_forks[fork_j])
    {
        if (!available_forks[fork_i])
        {
            printf("phil_%d is waiting for fork_%d | ", philosopher_number, fork_i);
        }

        if (!available_forks[fork_j])
        {
            printf("phil_%d is waiting for fork_%d | ", philosopher_number, fork_j);
        }

        pthread_cond_wait(&cond_t, &mutex);
    }

    available_forks[fork_i] = 0;
    printf("phil_%d takes fork_%d | ", philosopher_number, fork_i);
    available_forks[fork_j] = 0;
    printf("phil_%d takes fork_%d | ", philosopher_number, fork_j);

    pthread_mutex_unlock(&mutex);
}

void return_forks(int philosopher_number)
{
    int fork_i = philosopher_number;
    int fork_j = (fork_i + 1) % NUMBER_OF_THREADS;

    pthread_mutex_lock(&mutex);

    available_forks[fork_i] = 1;
    printf("phil_%d returns fork_%d | ", philosopher_number, fork_i);
    available_forks[fork_j] = 1;
    printf("phil_%d returns fork_%d | ", philosopher_number, fork_j);
    pthread_cond_signal(&cond_t);

    pthread_mutex_unlock(&mutex);
}

void *dining(void *params)
{
    parameters *p = (parameters *)params;

    int i = p->phil_num;
    while (completed_phils[i] < CYCLES)
    {
        think(i);
        printf("phil_%d finish thinking\n", i);
        pickup_forks(i);
        eat(i);
        printf("phil_%d finish eating\n", i);
        return_forks(i);
        completed_phils[i] += 1;
    }

    pthread_exit(0);
}

int main(int argc, const char *argv[])
{
    pthread_t phils[NUMBER_OF_THREADS];

    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&cond_t, 0);

    for (int p = 0; p < NUMBER_OF_THREADS; p++)
    {
        parameters *phil = (parameters *)malloc(sizeof(parameters));
        phil->phil_num = p;
        pthread_create(&phils[p], 0, dining, phil);
    }

    for (int p = 0; p < NUMBER_OF_THREADS; p++)
    {
        pthread_join(phils[p], 0);
    }

    printf("\n");
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        printf("phil_%d ", i);
        if (completed_phils[i] == CYCLES)
        {
            printf("finished dining\n");
        }
        else
        {
            printf("did not finish dining\n");
        }
    }

    return 0;
}