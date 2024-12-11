#include "eeprom.h"
#include <string.h>

static uint8_t eepromData[EEPROM_SIZE] = {0};

void eepromWrite(uint8_t address, const uint8_t* data, uint8_t length) {
    if (address + length <= EEPROM_SIZE) {
        memcpy(&eepromData[address], data, length);
    }
}

void eepromRead(uint8_t address, uint8_t* buffer, uint8_t length) {
    if (address + length <= EEPROM_SIZE) {
        memcpy(buffer, &eepromData[address], length);
    }
}

void setHardwareRevision(uint8_t revision) {
    eepromWrite(0, &revision, 1);
}

uint8_t getHardwareRevision(void) {
    uint8_t revision;
    eepromRead(0, &revision, 1);
    return revision;
}

void setSerialNumber(const char* serialNumber) {
    eepromWrite(1, (const uint8_t*)serialNumber, strlen(serialNumber) + 1);
}

void getSerialNumber(char* buffer, uint8_t bufferSize) {
    eepromRead(1, (uint8_t*)buffer, bufferSize);
}
