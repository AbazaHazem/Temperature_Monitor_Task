#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 10
#define SAMPLE_INTERVAL_US 100

typedef void (*TaskFunction)(void);

typedef struct {
    TaskFunction task;
    uint32_t executionTimeRemaining;
    uint32_t periodUs;
} Task;

void addTask(TaskFunction task, uint32_t periodUs);
void removeTask(TaskFunction task);
void timePartitionScheduler(void);

#endif // SCHEDULER_H
