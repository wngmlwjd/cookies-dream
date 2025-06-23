#ifndef PAUSESCENE_H                 // 헤더 파일 중복 포함 방지 시작
#define PAUSESCENE_H                 // PAUSESCENE_H 매크로 정의

#include "../sceneManager.h"        // 씬 관리자 헤더 포함

extern int buttons[];               // 버튼 핀 배열 외부 선언
extern int buttons_led[];           // 버튼 LED 핀 배열 외부 선언
extern LiquidCrystal lcd;           // LCD 객체 외부 선언
extern Scene currentScene;          // 현재 씬 변수 외부 선언

extern int speakerPin;              // 스피커 핀 번호 외부 선언

extern bool GAME_START;             // 게임 시작 상태 변수 외부 선언
extern bool isJumping_play;         // 점프 중 상태 변수 외부 선언

// 일시정지 씬 화면 출력 함수 선언
void showPauseScene();

// 일시정지 씬 상태 업데이트 함수 선언
void updatePauseScene();

#endif                            // 헤더 파일 중복 포함 방지 끝
