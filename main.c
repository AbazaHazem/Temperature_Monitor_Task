#include "scheduler.h"
#include "pubsub.h"
#include "eeprom.h"
#include <stdio.h>
#include <string.h>

void temperatureTask(void) {
    static float temp = 25.0;
    char message[64];
    snprintf(message, sizeof(message), "%.1f C", temp);
    publish("temperature", message);
    temp += 0.5;
}

void ledUpdateTask(void) {
    processPubSub();
}

void republisherTask(void) {
    static char lastMessage[64] = "25.0 C";  // Cache the last published message
    publish("temperature", lastMessage);   // Republish the last message
}

void serialNumberCallback(const char* message) {
    printf("Serial Number: %s\n", message);
}

int main() {
    setHardwareRevision(1);
    setSerialNumber("ABC1234");

    char serialNumber[16];
    getSerialNumber(serialNumber, sizeof(serialNumber));

    subscribe("temperature", serialNumberCallback);

    // Add tasks to the scheduler
    addTask(temperatureTask, 1000);      // Run temperature task every 1ms
    addTask(ledUpdateTask, 1000);       // Run LED update task every 1ms
    addTask(republisherTask, 100);      // Republish temperature message every 100 microseconds

    timePartitionScheduler();

    return 0;
}
