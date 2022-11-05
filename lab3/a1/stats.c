
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 3

typedef struct
{
    int value;
} result;

int size;
int *nums;

void *get_avg(void *res)
{
    result *r = (result *)res;
    int sum = 0;

    for (int i = 0; i < size; i++)
    {
        int num = *(nums + i);
        sum += num;
    }

    r->value = sum / size;
    pthread_exit(0);
}

void *get_min(void *res)
{
    result *r = (result *)res;
    int min = *nums;

    for (int i = 1; i < size; i++)
    {
        int num = *(nums + i);
        if (num < min)
        {
            min = num;
        }
    }

    r->value = min;
    pthread_exit(0);
}

void *get_max(void *res)
{
    result *r = (result *)res;
    int max = *nums;

    for (int i = 0; i < size; i++)
    {
        int num = *(nums + i);
        if (num > max)
        {
            max = num;
        }
    }

    r->value = max;
    pthread_exit(0);
}

int main(int argc, const char *argv[])
{
    argc--;
    argv++;

    size = argc;

    int _nums[size];
    for (int i = 0; i < size; i++)
    {
        int num = (int)strtol(argv[i], NULL, 10);
        _nums[i] = num;
    }

    nums = _nums;

    pthread_t workers[NUMBER_OF_THREADS];

    result *avg_res = (result *)malloc(sizeof(result));
    result *min_res = (result *)malloc(sizeof(result));
    result *max_res = (result *)malloc(sizeof(result));

    pthread_create(&workers[0], 0, get_avg, avg_res);
    pthread_create(&workers[1], 0, get_min, min_res);
    pthread_create(&workers[2], 0, get_max, max_res);

    for(int w = 0; w < NUMBER_OF_THREADS; w++){
        pthread_join(workers[w], NULL);
    }


    printf("The average value is %d\n", avg_res->value);
    printf("The minimym value is %d\n", min_res->value);
    printf("The maximum value is %d\n", max_res->value);

    return 0;
}
