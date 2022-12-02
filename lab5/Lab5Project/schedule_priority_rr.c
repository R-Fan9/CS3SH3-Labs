/**
 * schedule_priority_rr.c
 *
 * C program that implements
 * the Priority Round-Robin scheduling algorithm
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

// pointer to the struct containing the next task
struct node *tmp;

// integer variable to keep track of the highest priority task
int hp = -1;

// sequence counter of next available thread identifier
int nextTid = 0;

Task *pickNextTask();

// add a new task to the list of tasks
void add(char *name, int priority, int burst)
{
    // first create the new task
    Task *newTask = (Task *)malloc(sizeof(Task));

    newTask->tid = nextTid++;
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;

    // insert the new task into the task list
    insert(&head, newTask);
}

/**
 * Run the priority round-robin scheduler
 */
void schedule()
{

    // pointer to the current running task
    Task *current;

    // initialize tmp with the head of the task list
    tmp = head;

    // run until all tasks are finished
    while (head != NULL)
    {
        // pick a task
        current = pickNextTask();

        // if the burst time of the current task is greater than the time quantum
        if (current->burst > QUANTUM)
        {
            // run the task for a certain amount of time that is equal to the time quantum
            run(current, QUANTUM);

            /**
             * subtract time quantum from the time burst time of
             * the current task to keep track of the time that
             * the task has ran
             */
            current->burst -= QUANTUM;
        }
        // if the burst time of the current task is less than the time quantum
        else
        {
            // finish running the task
            run(current, current->burst);

            current->burst = 0;

            printf("Task %s finished.\n", current->name);
            // delete the task from the list
            delete (&head, current);
        }
    }
}

/**
 * Returns the next task selected to run.
 */
Task *pickNextTask()
{
    /**
     * pointer for iterating throught task list, 
     * initialize it to "tmp"
     */
    struct node *cur = tmp;
    // pointer to the next task that is going to be run
    Task *nextTask = tmp->task;

    // if there is more than one task in the list
    if (head->next != NULL)
    {
        /**
         * if the highest priority number has not been 
         * properly set or the next task's priority is 
         * less than the highest priority number
         */
        if (hp == -1 || nextTask->priority < hp)
        {
            // iterate through the list to find the number with the highest priority
            do
            {
                // if the end of the list has been reached
                if (cur->next == NULL)
                    // pointer loop back to the head of the list
                    cur = head;
                // otherwise, pointer to the next task in the list
                else
                    cur = cur->next;
                
                // assign the next task to be the current task if its priority is higher
                if (cur->task->priority > nextTask->priority)
                {
                    nextTask = cur->task;
                }
            
            /**
             * iterate through the task list until a task with the highest priority 
             * has been found or one cycle of iteration has completed 
             */
            } while (nextTask->priority != hp && cur->task->tid != tmp->task->tid);

            // update the highest priority number with the next task's priority
            hp = nextTask->priority;
        }

        /**
         * assign "tmp" to another task that is differ  
         * from the next task as next task can be deleted 
         * from the list after execution, which can cause 
         * "tmp" to be a dangling pointer
         */
        do
        {
            if (tmp->next == NULL)
                tmp = head;
            else
                tmp = tmp->next;

        } while (tmp->task->tid == nextTask->tid);
    }

    return nextTask;
}
