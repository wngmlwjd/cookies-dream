#ifndef EEPROM_H
#define EEPROM_H

byte read_eeprom(int address);  // EEPROM에서 1바이트 읽기
int read_eeprom_int(int address);  // EEPROM에서 2바이트 정수 읽기

void write_eeprom(int address, byte data);  // EEPROM에 1바이트 쓰기
void write_eeprom_int(int address, int data);  // EEPROM에 2바이트 정수 쓰기

int getBestScore(int stage);  // 특정 스테이지의 최고 점수 가져오기
void setBestScore(int stage, int score);  // 특정 스테이지의 최고 점수 저장하기

#endif
