#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedulers.h"
#include "list.h"
#include "cpu.h"

#define DISPATCHER_TIME 1
// Define the task list in this file
struct node *taskList = NULL;

// Adds a task to the list
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&taskList, newTask);
}

// Pick next task
Task *nextTask(){
    if(!taskList){
        return NULL;
    }

    struct node *temp = taskList;
    struct node *highestPriorityTask = temp;
    temp = temp->next;

    // Traverse the rest of the list
    while (temp != NULL){
        if (temp->task->priority > highestPriorityTask->task->priority){
            highestPriorityTask = temp;
        }
        else if (temp->task->priority == highestPriorityTask->task->priority){
            // If priorities are the same, use lexicographical order of the task names
            if (strcmp(temp->task->name, highestPriorityTask->task->name) < 0){
                highestPriorityTask = temp;
            }
        }
        temp = temp->next;
    }
    // Return the task with the highest priority
    return highestPriorityTask->task;
}

// Invokes the scheduler
void schedule()
{
    // Redirect stdout to a file
    freopen("output.txt", "a", stdout);

    printf("Priority: \n");
    int currTime = 0;
    int totalTaskTime = 0; // Total time tasks have been running
    int taskCount = 0;      // Count of processed tasks

    while (taskList)
    {
        Task *task = nextTask();
        taskCount++;

        run(task, task->burst);

        totalTaskTime += task->burst;
        currTime += task->burst;

        // Add dispatcher time
        if (taskList->next)
        {
            currTime += DISPATCHER_TIME;
        }

        delete (&taskList, task);
        free(task->name);
        free(task);
    }

    // Calculate CPU utilization
    // We subtract the dispatcher time only if there was more than one task
    float cpuUtil = 100.0 * ((float)totalTaskTime / (currTime - (taskCount > 1 ? DISPATCHER_TIME : 0)));
    printf("CPU Utilization: %.2f%%\n", cpuUtil);
    printf("\n\n");

    // Close the file to ensure all output is flushed to 'output.txt'
    fclose(stdout);
}

