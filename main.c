#include "scheduler.h"
#include "pubsub.h"
#include "eeprom.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// GPIO Mock Functions
void setGreenLED(int state) {
    printf("Green LED: %s\n", state ? "ON" : "OFF");
}

void setYellowLED(int state) {
    printf("Yellow LED: %s\n", state ? "ON" : "OFF");
}

void setRedLED(int state) {
    printf("Red LED: %s\n", state ? "ON" : "OFF");
}

// Temperature Sampling Task
void temperatureSamplingTask(void) {
    static uint16_t adcValue = 0;
    char message[64];

    uint8_t hardwareRevision = getHardwareRevision();
    float temperature = (hardwareRevision == 0) ? adcValue : adcValue / 10.0f;

    // Publish the temperature
    snprintf(message, sizeof(message), "%.1f", temperature);
    publish("temperature", message);

    // Simulate ADC Value Increment (mock data)
    adcValue = (adcValue + 5) % 200;  // Wrap at 200 for simulation
}

// LED Visualization Task
void ledVisualizationTask(void) {
    static char lastTemperature[64] = "0.0";
    float temperature;

    processPubSub(); // Process incoming messages

    sscanf(lastTemperature, "%f", &temperature);
    if (temperature >= 105.0f) {
        setRedLED(1);
        setYellowLED(0);
        setGreenLED(0);
    } else if (temperature >= 85.0f) {
        setRedLED(0);
        setYellowLED(1);
        setGreenLED(0);
    } else {
        setRedLED(0);
        setYellowLED(0);
        setGreenLED(1);
    }
}

// Republisher Task for Low Jitter
void republisherTask(void) {
    static char lastMessage[64] = "25.0";  // Cache the last published message

    // Republish the most recent message
    publish("temperature", lastMessage);
}

// Main Program
int main() {
    // Initialize EEPROM
    setHardwareRevision(0); // Set to Rev-A
    setSerialNumber("ABC1234");

    char serialNumber[16];
    getSerialNumber(serialNumber, sizeof(serialNumber));

    printf("Hardware Revision: %d\n", getHardwareRevision());
    printf("Serial Number: %s\n", serialNumber);

    // Subscribe to temperature topic
    subscribe("temperature", ledVisualizationTask);

    // Add tasks to the scheduler with defined on-periods
    addTask(temperatureSamplingTask, 1000, 0, 1000);  // Sampling every 1ms
    addTask(ledVisualizationTask, 1000, 0, 1000);     // Update LEDs every 1ms
    addTask(republisherTask, 1000, 980, 1000);        // Republish within 980-1000 interval

    // Start scheduler
    timePartitionScheduler();

    return 0;
}
