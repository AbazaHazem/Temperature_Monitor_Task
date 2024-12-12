#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

#define EEPROM_SIZE 64

// Mock EEPROM Read/Write
void eepromWrite(uint8_t address, const uint8_t* data, uint8_t length);
void eepromRead(uint8_t address, uint8_t* buffer, uint8_t length);

// High-level EEPROM utilities
void setHardwareRevision(uint8_t revision);
uint8_t getHardwareRevision(void);
void setSerialNumber(const char* serialNumber);
void getSerialNumber(char* buffer, uint8_t bufferSize);

#endif // EEPROM_H
