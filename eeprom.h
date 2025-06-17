#ifndef EEPROM_H
#define EEPROM_H

byte read_eeprom(int address);
int read_eeprom_int(int address);

void write_eeprom(int address, byte date);
void write_eeprom_int(int address, int data);

int getBestScore(int stage);
void setBestScore(int stage, int score);

#endif