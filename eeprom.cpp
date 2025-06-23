#include <Arduino.h>       // 아두이노 기본 라이브러리 포함
#include <EEPROM.h>        // EEPROM 읽기/쓰기 라이브러리 포함

#include "eeprom.h"        // 사용자 정의 eeprom 헤더 파일 포함

// 지정한 주소에서 1바이트 데이터를 읽어 반환하는 함수
byte read_eeprom(int address) {
    return EEPROM.read(address);
}

// 지정한 주소에 1바이트 데이터를 쓰는 함수
void write_eeprom(int address, byte data) {
    EEPROM.write(address, data);
}

// 지정한 주소에서 2바이트 데이터를 읽어 int 형으로 반환하는 함수
int read_eeprom_int(int address) {
    byte data1 = EEPROM.read(address);           // 상위 바이트 읽기
    byte data2 = EEPROM.read(address + 1);       // 하위 바이트 읽기
    return ((data1 << 8) | data2);               // 상위, 하위 바이트 합쳐서 int로 반환
}

// 지정한 주소에 int형 데이터를 2바이트로 나누어 저장하는 함수
void write_eeprom_int(int address, int data) {
    EEPROM.write(address, data >> 8);            // 상위 바이트 저장
    EEPROM.write(address + 1, data & 0xFF);      // 하위 바이트 저장
}

// 특정 스테이지의 최고 점수를 EEPROM에서 읽어 반환하는 함수
int getBestScore(int stage) {
    return read_eeprom_int(stage * 2);           // 각 스테이지는 2바이트씩 할당
}

// 특정 스테이지의 최고 점수를 EEPROM에 저장하는 함수
void setBestScore(int stage, int score) {
    write_eeprom_int(stage * 2, score);          // 각 스테이지는 2바이트씩 할당
}
