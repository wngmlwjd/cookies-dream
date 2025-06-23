#include <openGLCD.h>             // GLCD 제어 라이브러리 포함
#include <LiquidCrystal.h>        // LCD 제어 라이브러리 포함

#include "../textUtils.h"         // 텍스트 유틸리티 함수 포함

#include "startScene.h"           // 시작 씬 헤더 포함

// LCD에 출력할 텍스트 배열, 각 줄의 위치와 문자열 포함
LcdTextLine startText[] = {
    {0, 0, "Press Button"},       // 첫 번째 줄 (0,0 위치)
    {6, 1, "to Start!!"}          // 두 번째 줄 (6,1 위치)
};

// 시작 씬 화면 출력 함수
void showStartScene() {
    GLCD.ClearScreen();            // GLCD 화면 클리어
    lcd.clear();                  // LCD 화면 클리어

    digitalWrite(buttons_led[0], HIGH);  // 첫 번째 버튼 LED ON

    drawCenteredText(20, "Cookie's Dream");  // GLCD 중앙에 게임 제목 출력 (y=20)
    drawCenteredText(40, "start");            // GLCD 중앙에 "start" 출력 (y=40)

    setupBlinkingText(40, "start");            // "start" 텍스트 깜빡이기 설정(GLCD)
    setupLcdBlinkingText(startText, 2);        // LCD 텍스트 2줄 깜빡이기 설정
}

// 시작 씬 상태 업데이트 함수
void updateStartScene() {
    updateBlinkingText();           // GLCD 깜빡임 텍스트 업데이트
    updateLcdBlinkingText(2);      // LCD 깜빡임 텍스트 2줄 업데이트

    if (digitalRead(buttons[0])) {  // 첫 번째 버튼이 눌렸을 경우
        changeScene(GAME_READY);    // 씬을 스테이지 선택 씬으로 변경 (사용자 요청에 따라 명칭 변경 필요)
    }
}
