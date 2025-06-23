#ifndef STARTSCENE_H                   // 헤더 파일 중복 포함 방지 시작
#define STARTSCENE_H                   // STARTSCENE_H 매크로 정의

#include "../sceneManager.h"          // 씬 관리자 헤더 포함

extern int buttons[];                 // 버튼 핀 배열 외부 선언
extern int buttons_led[];             // 버튼 LED 핀 배열 외부 선언
extern LiquidCrystal lcd;             // LCD 객체 외부 선언
extern Scene currentScene;            // 현재 씬 변수 외부 선언

// 시작 씬 화면 출력 함수 선언
void showStartScene();

// 시작 씬 상태 업데이트 함수 선언
void updateStartScene();

#endif                              // 헤더 파일 중복 포함 방지 끝
