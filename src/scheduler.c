#include "scheduler.h"
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define MAX_TASKS 10

typedef struct {
    TaskFunction task;
    uint32_t executionTimeRemaining;
    uint32_t periodUs;
    uint32_t onStartTimeUs;
    uint32_t onEndTimeUs;
} Task;

static Task scheduler[MAX_TASKS] = {0};
static uint8_t taskCount = 0;

void addTask(TaskFunction task, uint32_t periodUs, uint32_t onStartTimeUs, uint32_t onEndTimeUs) {
    if (periodUs == 0 || onEndTimeUs <= onStartTimeUs || taskCount >= MAX_TASKS) {
        printf("Error: Invalid task parameters or task limit reached.\n");
        return;
    }
    scheduler[taskCount].task = task;
    scheduler[taskCount].executionTimeRemaining = 0;  // Start immediately on first cycle
    scheduler[taskCount].periodUs = periodUs;
    scheduler[taskCount].onStartTimeUs = onStartTimeUs;
    scheduler[taskCount].onEndTimeUs = onEndTimeUs;
    taskCount++;
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
    printf("Error: Task not found.\n");
}

void timePartitionScheduler(void) {
    uint32_t currentTime = 0;

    while (1) {
        for (uint8_t i = 0; i < taskCount; i++) {
            if (scheduler[i].executionTimeRemaining == 0) {
                uint32_t currentMicroTime = currentTime % scheduler[i].periodUs;

                if (currentMicroTime >= scheduler[i].onStartTimeUs && currentMicroTime < scheduler[i].onEndTimeUs) {
                    scheduler[i].task();
                }

                scheduler[i].executionTimeRemaining = scheduler[i].periodUs;
            }

            scheduler[i].executionTimeRemaining -= SAMPLE_INTERVAL_US;
        }

        usleep(SAMPLE_INTERVAL_US);
        currentTime += SAMPLE_INTERVAL_US;
    }
}
