#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <LiquidCrystal.h>

enum Scene {
  START, // 시작 화면
  GAME_READY, // 스테이지 선택 화면
  GAME_PLAY, // 게임 플레이 화면
  PAUSE, // 일시 정지 화면
  END // 게임 종료 화면
};

extern int buttons[]; // 버튼 입력 핀 배열
extern int buttons_led[]; // 버튼 LED 핀 배열
extern LiquidCrystal lcd; // LCD 객체

void changeScene(Scene newScene); // 씬 전환 함수
void updateScene(); // 현재 씬 업데이트 함수

#endif
