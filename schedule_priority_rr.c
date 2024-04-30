#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedulers.h"
#include "list.h"
#include "cpu.h"
#include "stdbool.h"

#define TIME_QUANTUM 10
#define DISPATCHER_TIME 1

struct node *taskList = NULL;

// add a task to the list
void add(char*name, int priority, int burst){
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&taskList, newTask);
}

// pick next task to execute with priority round robin scheduling
Task * nextTask(){
    if(!taskList){
        return NULL;
    }
    struct node *temp = taskList;
    struct node *highest_priority_task = temp;
    temp = temp->next;

    // Find the task with the highest priority
    while (temp != NULL)
    {
        if (temp->task->priority > highest_priority_task->task->priority)
        {
            highest_priority_task = temp;
        }
        temp = temp->next;
    }

    // Now we find the first task with this priority in lexicographic order
    int highest_priority = highest_priority_task->task->priority;
    struct node *current = taskList;
    struct node *selected_task = NULL;

    while (current != NULL)
    {
        if (current->task->priority == highest_priority)
        {
            if (selected_task == NULL || strcmp(current->task->name, selected_task->task->name) < 0)
            {
                selected_task = current;
            }
        }
        current = current->next;
    }

    // Return the task with the highest priority and lexicographically smallest name
    return selected_task->task;
}

void schedule(){
    freopen("output.txt", "a", stdout);

    printf("Priority Round Robin: \n");

    int currTime = 0;
    int totalTaskTime = 0;
    int taskCount = 0;

    while(taskList){
        Task *task = nextTask();
        taskCount++;

        int slice = task->burst < TIME_QUANTUM ? task->burst : TIME_QUANTUM;
        run(task, slice); // run the task
        task->burst -= slice;
        totalTaskTime += slice;

        if(task->burst <= 0){
            delete(&taskList, task);
            free(task->name);
            free(task);
        } else{
            // Move the task to the end of the list
            delete(&taskList, task);
            insert(&taskList, task);
        } 

        if(taskList != NULL){
            currTime += DISPATCHER_TIME;
        }
    }
    // Adjust the current time by subtracting the dispatcher time following the last task
    if (taskCount > 0){
        currTime += totalTaskTime + DISPATCHER_TIME * (taskCount - 1);
    }

    // Calculate CPU utilization
    float cpu_utilization = 100.0 * ((float)totalTaskTime / currTime);
    printf("CPU Utilization: %.2f%%\n", cpu_utilization);

    printf("\n\n");

    // Close the file to ensure all output is flushed to 'output.txt'
    fclose(stdout);
}