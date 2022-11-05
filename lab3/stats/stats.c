/**
 * stats.c
 *
 * C program that calculates
 * various statistical values
 * for a list of numbers using
 * multithread
 *
 * fanh11@mcmaster.ca
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// define the number of worker threads
#define NUMBER_OF_THREADS 3

/**
 * define a data structure that
 * store the value return by
 * a function
 */
typedef struct
{
    int size;
    int *nums;
} parameters;

// /**
//  * global variable that
//  * stores the size of
//  * the input list
//  */
// int size;

// /**
//  * global pointer that
//  * points to the address
//  * of the input list
//  */
// int *nums;

double avg_num;
int min_num;
int max_num;

/**
 * a function that determines
 * average of the numbers
 * from the list
 */
void *get_avg(void *params)
{
    parameters *p = (parameters *)params;
    int size = p->size;
    int *nums = p->nums;
    int sum = 0;

    for (int i = 0; i < size; i++)
    {
        int num = *(nums + i);
        sum += num;
    }

    avg_num = (double)sum / size;
    pthread_exit(0);
}

/**
 * a function that determines
 * minimum of the numbers
 * from the list
 */
void *get_min(void *params)
{
    parameters *p = (parameters *)params;
    int size = p->size;
    int *nums = p->nums;
    int min = *nums;

    for (int i = 1; i < size; i++)
    {
        int num = *(nums + i);
        if (num < min)
        {
            min = num;
        }
    }

    min_num = min;
    pthread_exit(0);
}

void *get_max(void *params)
{
    parameters *p = (parameters *)params;
    int size = p->size;
    int *nums = p->nums;
    int max = *nums;

    for (int i = 0; i < size; i++)
    {
        int num = *(nums + i);
        if (num > max)
        {
            max = num;
        }
    }

    max_num = max;
    pthread_exit(0);
}

int main(int argc, const char *argv[])
{
    argc--;
    argv++;

    // size = argc;

    int _nums[argc];
    for (int i = 0; i < argc; i++)
    {
        int num = (int)strtol(argv[i], NULL, 10);
        _nums[i] = num;
    }

    // nums = _nums;

    pthread_t workers[NUMBER_OF_THREADS];

    // result *avg_res = (result *)malloc(sizeof(result));
    // result *min_res = (result *)malloc(sizeof(result));
    // result *max_res = (result *)malloc(sizeof(result));
    parameters *data = (parameters *)malloc(sizeof(parameters));
    data->size = argc;
    data->nums = _nums;

    // pthread_create(&workers[0], 0, get_avg, avg_res);
    // pthread_create(&workers[1], 0, get_min, min_res);
    // pthread_create(&workers[2], 0, get_max, max_res);
    pthread_create(&workers[0], 0, get_avg, data);
    pthread_create(&workers[1], 0, get_min, data);
    pthread_create(&workers[2], 0, get_max, data);

    for (int w = 0; w < NUMBER_OF_THREADS; w++)
    {
        pthread_join(workers[w], NULL);
    }

    // printf("The average value is %d\n", avg_res->value);
    // printf("The minimym value is %d\n", min_res->value);
    // printf("The maximum value is %d\n", max_res->value);
    printf("The average value is %f\n", avg_num);
    printf("The minimym value is %d\n", min_num);
    printf("The maximum value is %d\n", max_num);

    return 0;
}
