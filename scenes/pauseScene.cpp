#include <openGLCD.h>               // GLCD 제어 라이브러리 포함
#include <LiquidCrystal.h>          // LCD 제어 라이브러리 포함

#include "../textUtils.h"           // 텍스트 유틸리티 함수 포함
#include "../led.h"                 // LED 제어 함수 포함

#include "gamePlayScene.h"          // 게임 플레이 씬 헤더 포함
#include "pauseScene.h"             // 일시정지 씬 헤더 포함

// LCD에 출력할 일시정지 메시지 텍스트 배열
LcdTextLine pauseText[] = {
    {0, 0, "Game Paused!!"},         // 첫 번째 줄 (0,0 위치)
};

// 일시정지 씬 화면 출력 함수
void showPauseScene() {
    lcd.clear();                     // LCD 화면 클리어
    setupLcdBlinkingText(pauseText, 1);  // LCD 텍스트 1줄 깜빡임 설정
    lcd.setCursor(0, 1);             // LCD 두 번째 줄 첫 칸으로 커서 이동
    lcd.print("Resume  |   Exit");   // 재개 및 종료 안내 문구 출력
}

// 일시정지 씬 상태 업데이트 함수
void updatePauseScene() {
    updateLcdBlinkingText(2);        // LCD 깜빡임 텍스트 2줄 업데이트

    digitalWrite(buttons_led[2], HIGH);  // Resume 버튼 LED ON
    digitalWrite(buttons_led[3], HIGH);  // Exit 버튼 LED ON

    if (digitalRead(buttons[2]) == LOW) {    // Resume 버튼 눌림 감지 (LOW 활성화)
        changeScene(GAME_PLAY);                // 씬을 게임 플레이 씬으로 변경
    }
    else if (digitalRead(buttons[3]) == LOW) { // Exit 버튼 눌림 감지 (LOW 활성화)
        GAME_START = false;                    // 게임 시작 상태 false로 변경
        isJumping_play = false;                // 점프 상태 false로 변경

        noTone(speakerPin);                    // 스피커 소리 중지

        clear_segments();                      // 7세그먼트 초기화 함수 호출
        led_off();                            // LED 모두 끄기 함수 호출

        changeScene(START);                    // 씬을 시작 씬으로 변경
    }
}
