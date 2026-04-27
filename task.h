/// Task structure 
typedef struct {
    int id;
    char content[256];
    int status;
} Task;

/// Creates a new task with the given contents
void addTask(char *contents);

/// Marks the given task with the new state 
void markTaskState(int taskId, int state);

/// Prints all the tasks 
void printTasks(); 
