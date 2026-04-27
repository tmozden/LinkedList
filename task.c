#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "task.h"


int taskCount = 0; 

Task tasks[100];

/// Creates a new task with the given contents
void addTask(char *contents) {
   Task newTask; 
   newTask.id = taskCount;
   strncpy(newTask.content, contents, sizeof(newTask.content));
   newTask.status = 0; 
   tasks[taskCount] = newTask;

   taskCount ++;
}

void markTaskState(int taskId, int state) {
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == taskId) {
            tasks[i].status = state;
            break;
        }
    }
}


void printTasks() {
    printf("## Tasks\n");
    char buffer[512];

    for(int i = 0; i < taskCount; i++) {
        char *done = tasks[i].status ? "x" : " ";
        sprintf(buffer, "  - [%s] %d: %s", done, tasks[i].id, tasks[i].content);
        printf("%s", buffer);
    }
}
