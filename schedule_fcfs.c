#include <stdio.h>
#include <stdlib.h>
#include "schedulers.h"
#include "list.h"
#include "cpu.h"
#include <string.h>
#include "stdbool.h"

#define DISPATCHER_TIME 1

struct node *taskList = NULL;
int tid = 0;

// add a task to the list
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&taskList, newTask);
}

// Helper function to compare task names
bool compareTaskName(char *a, char *b) {
    return strcmp(a, b) < 0;
}

// pick the next task from the list
Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!taskList){
    return NULL;
  }

  struct node *temp = taskList;
  Task *bestSoFar = temp;

  while (temp != NULL) {
    if (comesBefore(temp->task->name, bestSoFar->name))
      bestSoFar = temp->task;
    temp = temp->next;
  }
  
  return bestSoFar;
}

void schedule(){
    freopen("output.txt", "a", stdout);
    printf("First come first serve: \n");
    int currTime = 0;
    int totalTaskTime = 0;
    int taskCount = 0;

    while(taskList){
        Task *task = nextTask();
        run(task, task->burst);       
        currTime += task->burst;    
        totalTaskTime += task->burst; 
        taskCount++;                   
        delete (&taskList, task);       
        free(task);  
    }

    // Adjust for dispatcher time
    currTime += DISPATCHER_TIME * (taskCount - 1);

    // Calculate CPU utilization
    float cpuUtil = 100 * ((float)totalTaskTime / currTime);
    printf("CPU Utilization: %.2f%%\n", cpuUtil);
    printf("\n\n");
    fclose(stdout);
}