#include "schedulers.h"
#include "list.h"
#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TIME_QUANTUM 10
#define DISPATCHER_TIME 1

struct node *taskList = NULL;

// add a task to the list
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    newTask->burst = burst;

    // insert new node into list
    if(taskList == NULL || strcmp(taskList->task->name, newTask->name) > 0){
        newNode->next = taskList;
        taskList = newNode;
    } else {
        struct node *curr = taskList;
        while (curr->next != NULL && strcmp(curr->next->task->name, newTask->name) < 0){
            curr = curr->next;
        }
        // insert node in correct place
        newNode->next = curr->next;
        curr->next = newNode;  
    }
}

void schedule(){
    // Redirect stdout to a file
    freopen("output.txt", "a", stdout);

    printf("Round Robin: \n");
    struct node *currNode = taskList;
    int currTime = 0;
    int totalTaskTime = 0; // Total time tasks have been running
    int tasksProcessed = 0; 

    while(currNode != NULL){
        Task *task = currNode->task;
        tasksProcessed++;

        int slice = task->burst > TIME_QUANTUM ? TIME_QUANTUM : task->burst;
        run(task, slice);
        task->burst -= slice;
        totalTaskTime += slice;

        // if task is finished remove it from list
        if(task->burst == 0){
            struct node *temp = currNode;
            currNode = currNode->next;
            if(temp == taskList){ // if head is being removed
                taskList = currNode;
            } else {
                struct node *prev = taskList;
                while(prev->next != temp){
                    prev = prev->next;
                }
                prev->next = currNode;
            }
            free(temp->task->name);
            free(temp->task);
            free(temp);

        } else{
            // moving task to end of list
            struct node *temp = currNode;
            while(temp->next){
                temp = temp->next;
            }
            if(currNode != temp){
                temp->next = currNode;     // Put current at the end
                taskList = currNode->next; // New head is next
                currNode->next = NULL;     // Current is now last, so next is NULL
                currNode = taskList;       // continue with new head
            }else{
                currNode = currNode->next;
            }
        }
        // Account for dispatcher time unless it's the last task
        if (taskList != NULL){
            currTime += DISPATCHER_TIME;
        }
    }
    // Calculate CPU utilization
    currTime += totalTaskTime; // Total time is all tasks' time plus dispatcher time
    float cpuUtil = 100.0 * ((float)totalTaskTime / currTime);
    printf("CPU Utilization: %.2f%%\n", cpuUtil);

    printf("\n\n");

    // Close the file to flush all output
    fclose(stdout);
}