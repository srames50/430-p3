#include <stdio.h>
#include <stdlib.h>
#include "schedulers.h"
#include "list.h"
#include "cpu.h"

// Define the task list in this file
struct node *taskList = NULL;

// Add a task to the list
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&taskList, newTask);
}

// pick next task using SJF
Task *newTask(){
    if(!taskList){
        return NULL;
    }
    struct node *temp = taskList;
    struct node *shortest = taskList;
    temp = temp->next;

    while(temp){
        if(temp->task->burst < shortest->task->burst){
            shortest = temp;
        }
        temp = temp->next;
    }

    return shortest->task;
}

// Schedule tasks using SJF
void schedule(){
    freopen("output.txt", "a", stdout);
    printf("Shortest Job First:\n");

    int totalBurstTime = 0;
    int currTime = 0;
    int tasksRun = 0;

    while(taskList){
        Task *task = nextTask();
        run(task, task->burst); // Run the task
        totalBurstTime += task->burst;
        currTime += task->burst; // Add burst time to current time
        delete (&taskList, task);    // Remove it from the list
        free(task);                  // Free the memory for task
        tasksRun++;
    }

    // Subtract dispatcher time for the last task as it doesn't involve a context switch
    if (tasksRun > 1)
    {
        currTime += (tasksRun - 1); // Add dispatcher time
    }

    float cpuUtil = 100 * ((float)totalBurstTime / currTime);
    printf("CPU Utilization: %.2f%%\n", cpuUtil);

    printf("\n\n");
    fclose(stdout);
}