/**
 * Implementation of various scheduling algorithms.
 *
 * Priority Round-robin scheduling
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *head = NULL;

// pointer to the struct containing the next task
struct node *tmp;

int hp = -1;

Task *pickNextTask();

// add a new task to the list of tasks
void add(char *name, int priority, int burst)
{
    // first create the new task
    Task *newTask = (Task *)malloc(sizeof(Task));

    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;

    // insert the new task into the list of tasks
    insert(&head, newTask);
}

/**
 * Run the priority round-robin scheduler
 */
void schedule()
{
    Task *current;

    tmp = head;

    while (head != NULL)
    {
        current = pickNextTask();

        if (current->burst > QUANTUM)
        {
            run(current, QUANTUM);

            current->burst -= QUANTUM;
        }
        else
        {
            run(current, current->burst);

            current->burst = 0;

            printf("Task %s finished.\n", current->name);
            delete (&head, current);
        }
    }
}

/**
 * Returns the next task selected to run.
 */
Task *pickNextTask()
{
    struct node *cur = tmp;
    Task *nextTask = tmp->task;

    if (head->next != NULL)
    {
        if (hp == -1 || nextTask->priority < hp)
        {
            do
            {
                if (cur->next == NULL)
                    cur = head;
                else
                    cur = cur->next;

                if (cur->task->priority > nextTask->priority)
                {
                    nextTask = cur->task;
                }

            } while (nextTask->priority != hp && cur->task->name != tmp->task->name);

            hp = nextTask->priority;
        }

        do
        {
            if (tmp->next == NULL)
                tmp = head;
            else
                tmp = tmp->next;

        } while (tmp->task->name == nextTask->name);
    }

    return nextTask;
}
