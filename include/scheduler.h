#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 10
#define SAMPLE_INTERVAL_US 100

typedef void (*TaskFunction)(void);

void addTask(TaskFunction task, uint32_t periodUs, uint32_t onStartTimeUs, uint32_t onEndTimeUs);
void removeTask(TaskFunction task);
void timePartitionScheduler(void);

#endif // SCHEDULER_H

