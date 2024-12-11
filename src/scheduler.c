#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define MAX_TASKS 10
#define SAMPLE_INTERVAL_US 100

typedef void (*TaskFunction)(void);

typedef struct {
    TaskFunction task;
    uint32_t executionTimeRemaining;
    uint32_t periodUs;
} Task;

Task scheduler[MAX_TASKS] = {0};
uint8_t taskCount = 0;

void addTask(TaskFunction task, uint32_t periodUs) {
    if (periodUs == 0) {
        printf("Error: Task period cannot be zero.\n");
        return;
    }
    if (taskCount < MAX_TASKS) {
        scheduler[taskCount].task = task;
        scheduler[taskCount].executionTimeRemaining = periodUs;
        scheduler[taskCount].periodUs = periodUs;
        taskCount++;
    } else {
        printf("Task limit reached.\n");
    }
}

void removeTask(TaskFunction task) {
    for (uint8_t i = 0; i < taskCount; i++) {
        if (scheduler[i].task == task) {
            for (uint8_t j = i; j < taskCount - 1; j++) {
                scheduler[j] = scheduler[j + 1];
            }
            taskCount--;
            printf("Task removed successfully.\n");
            return;
        }
    }
    printf("Task not found.\n");
}

void timePartitionScheduler(void) {
    while (1) {
        for (uint8_t i = 0; i < taskCount; i++) {
            if (scheduler[i].executionTimeRemaining == 0) {
                scheduler[i].task();
                scheduler[i].executionTimeRemaining = scheduler[i].periodUs;
            }
            scheduler[i].executionTimeRemaining -= SAMPLE_INTERVAL_US;
        }
        usleep(SAMPLE_INTERVAL_US);
    }
}
