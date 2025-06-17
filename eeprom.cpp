#include <Arduino.h>
#include <EEPROM.h>

#include "eeprom.h"

byte read_eeprom(int address) {
    return EEPROM.read(address);
}

void write_eeprom(int address, byte data) {
    EEPROM.write(address, data);
}

int read_eeprom_int(int address) {
    byte data1 = EEPROM.read(address);
    byte data2 = EEPROM.read(address + 1);
    return ((data1 << 8) | data2);
}

void write_eeprom_int(int address, int data) {
    EEPROM.write(address, data >> 8);        // 상위 바이트
    EEPROM.write(address + 1, data & 0xFF);  // 하위 바이트
}

int getBestScore(int stage) {
    return read_eeprom_int(stage * 2);
}

void setBestScore(int stage, int score) {
    write_eeprom_int(stage * 2, score);
}
