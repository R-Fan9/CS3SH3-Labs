/**
 * schedule_sjf.c
 *
 * C program that implements
 * the Shortest Job First scheduling algorithm 
 *
 * fanh11@mcmaster.ca
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

// pointer to the start of the task list
struct node *head = NULL;

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
 * Run the shortest job first scheduler
 */
void schedule()
{
    // pointer to the current running task
    Task *current;

    // run until there is no more tasks in the list
    while (head != NULL)
    {
        // pick a task
        current = pickNextTask();

        // run the task
        run(current, current->burst);

        // after the task is finished, delete it from the list
        delete (&head, current);
    }
}

/**
 * Returns the next task selected to run.
 */
Task *pickNextTask()
{
    // pointer to each task in the list for iteration
    struct node *temp;
    // pointer to the next shorest brust time task
    Task *nextTask = head->task;

    // start with the head of the list
    temp = head->next;

    // iterate through the task list
    while (temp != NULL)
    {
        // assign the next task to be the current task if its burst time is shorter
        if (temp->task->burst < nextTask->burst)
            nextTask = temp->task;

        temp = temp->next;
    }

    return nextTask;
}
