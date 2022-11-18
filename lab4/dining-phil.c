/**
 * dining-phil.c
 *
 * C program that simulates
 * dining philosophers problem
 *
 * fanh11@mcmaster.ca
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * defines the number of threads, 
 * each thread represent a philosopher
*/
#define NUMBER_OF_THREADS 5
/**
 * defines the number of cycles 
 * that a philosopher must complete, 
 * a cycle starts from thinking, 
 * being hungry, and ends with 
 * finish eating
*/
#define CYCLES 2

/**
 * custom data type to represent 
 * the number of a philosopher 
*/
typedef struct
{
    int phil_num;
} parameters;

//an array to keep track of the number 
//cycles that the philosophers completed
int completed_phils[] = {0, 0, 0, 0, 0};
//an array to keep track of the availability
//of the forks 
int available_forks[] = {1, 1, 1, 1, 1};

//a mutex lock
pthread_mutex_t mutex;
//a conditon variable
pthread_cond_t cond_t;

//displays the status of each fork
void display_forks()
{
    printf("\n");
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        printf("fork_%d: %s\n", i, available_forks[i] ? "free" : "taken");
    }
    printf("\n");
}

/**
 * simulates the thinking state 
 * of a philosopher by sleeping 
 * 1 to 3 seconds 
*/
void think(int philosopher_number)
{
    int r = rand() % 3 + 1;

    printf("phil_%d is thinking for %d secs | ", philosopher_number, r);
    sleep(r);
}

/**
 * simulates the eating state 
 * of a philosopher by sleeping 
 * 1 to 3 seconds 
*/
void eat(int philosopher_number)
{
    int r = rand() % 3 + 1;

    printf("phil_%d is eating for %d secs | ", philosopher_number, r);
    sleep(r);
}

/**
 * simulates the state where
 * a philosopher is trying to 
 * pick up 2 neighbouring forks to eat
*/
void pickup_forks(int philosopher_number)
{
    //the index of the first fork that a philosopher tries to pick up
    int fork_i = philosopher_number;
    //the index of the second fork that a philosopher tries to pick up
    int fork_j = (fork_i + 1) % NUMBER_OF_THREADS;

    //acquires the mutex lock to protect critical section
    pthread_mutex_lock(&mutex);
    //shows the availablity status of each fork
    display_forks();

    //wait untill both forks are returned
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

        //release the mutex lock temporarily using the condition variable 
        //for others to return the forks
        pthread_cond_wait(&cond_t, &mutex);
    }

    //once both forks are available
    //takes the first fork
    available_forks[fork_i] = 0;
    printf("phil_%d takes fork_%d | ", philosopher_number, fork_i);
    //takes the second fork
    available_forks[fork_j] = 0;
    printf("phil_%d takes fork_%d | ", philosopher_number, fork_j);

    //release the mutex lock
    pthread_mutex_unlock(&mutex);
}

/**
 * simulates the state where
 * a philosopher is returning the 
 * forks after eating
*/
void return_forks(int philosopher_number)
{
    //the indexj of first fork to be returned
    int fork_i = philosopher_number;
    //the indexj of second fork to be returned
    int fork_j = (fork_i + 1) % NUMBER_OF_THREADS;

    //acquire the mutex lock
    pthread_mutex_lock(&mutex);
    //returns the first fork
    available_forks[fork_i] = 1;
    printf("phil_%d returns fork_%d | ", philosopher_number, fork_i);
    //returns the second fork
    available_forks[fork_j] = 1;
    printf("phil_%d returns fork_%d | ", philosopher_number, fork_j);
    //sin
    pthread_cond_signal(&cond_t);

    //release the mutex lock
    pthread_mutex_unlock(&mutex);
}

/**
 * simulates the whole dining 
 * process of a philosopher
*/
void *dining(void *params)
{
    //takes the number of a philosopher 
    //from the input parameter
    parameters *p = (parameters *)params;
    int i = p->phil_num;

    //each philosopher must complete 
    //certain number of cycles define 
    //above
    while (completed_phils[i] < CYCLES)
    {
        //a philosopher is thinking
        think(i);
        printf("phil_%d is now hungry\n", i);
        //after thinking, the philsopher is 
        //hungry and tries to pick up 2
        //forks 
        pickup_forks(i);
        //after picking up the forks, 
        //the philosopher starts eating
        eat(i);
        printf("phil_%d finish eating\n", i);
        //after eating, the philosopher 
        //returns the forks
        return_forks(i);
        //after the philosopher returns the 
        //forks, a cycle is completed
        completed_phils[i] += 1;
    }

    pthread_exit(0);
}

int main(int argc, const char *argv[])
{
    //initialize an array of 5 threads
    pthread_t phils[NUMBER_OF_THREADS];

    //initialize the mutex lock
    pthread_mutex_init(&mutex, 0);
    //initialize the condition variable
    pthread_cond_init(&cond_t, 0);

    //create 5 threads to simulate dining 
    //process of the philosophers
    for (int p = 0; p < NUMBER_OF_THREADS; p++)
    {
        //create input parameter that stores
        //the number of a philosopher
        parameters *phil = (parameters *)malloc(sizeof(parameters));
        phil->phil_num = p;
        pthread_create(&phils[p], 0, dining, phil);
    }

    //wait for each philosopher to finish the 
    //dining process
    for (int p = 0; p < NUMBER_OF_THREADS; p++)
    {
        pthread_join(phils[p], 0);
    }

    //displays the availability status 
    //of each work after all philosopher 
    //have finished dining
    printf("\nforks availability status:");
    display_forks();

    //displays the number of cycles that each 
    //philosopher has completed
    printf("philosophers dining status:\n");
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        printf("phil_%d finished dining %d times\n", i, completed_phils[i]);
    }

    return 0;
}