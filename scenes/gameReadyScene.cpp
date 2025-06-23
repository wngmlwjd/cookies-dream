#include <LiquidCrystal.h>              // LCD 제어 라이브러리 포함
#include <openGLCD.h>                   // GLCD 제어 라이브러리 포함

#include "../sceneManager.h"            // 씬 관리자 헤더 포함
#include "../cookie.h"                  // 쿠키 관련 함수 및 변수 포함
#include "../textUtils.h"               // 텍스트 유틸리티 함수 포함

#include "gamePlayScene.h"              // 게임 플레이 씬 헤더 포함
#include "gameReadyScene.h"             // 게임 준비(스테이지 선택) 씬 헤더 포함

// LCD에 출력할 스테이지 선택 안내 텍스트 배열
LcdTextLine readyText[] = {
    {0, 0, "Select Stage!!"},         // 첫 번째 줄 (0,0 위치)
    {0, 1, "Up|Down|Ok|Exit"}         // 두 번째 줄 (0,1 위치)
};

int stageNum = 3;                     // 전체 스테이지 수
stageName stageList[] = {             // 스테이지 리스트와 각 스테이지의 위치(line)와 이름
    {10, "Stage 1"},
    {25, "Stage 2"},
    {40, "Stage 3"}
};

// 게임 준비 씬 배경 그리기 함수
void drawBackground_ready() {
    GLCD.ClearScreen();               // GLCD 화면 클리어

    drawGround();                    // 땅 그리기 함수 호출 (외부 정의 필요)

    int x = 10;

    drawSquareCookie(x, cookieY);    // 왼쪽에 네모난 쿠키 그리기
    drawRoundCookie(116 - x, cookieY);  // 오른쪽에 동그란 쿠키 그리기

    // 각 스테이지 이름 중앙 정렬 텍스트로 출력
    for (int i = 0; i < stageNum; i++) {
        drawCenteredText(stageList[i].line, stageList[i].name);
    }
}

// 게임 준비 씬 화면 출력 함수
void showGameReadyScene() {
    GLCD.ClearScreen();               // GLCD 클리어
    lcd.clear();                     // LCD 클리어

    // 모든 버튼 LED ON
    digitalWrite(buttons_led[0], HIGH);
    digitalWrite(buttons_led[1], HIGH);
    digitalWrite(buttons_led[2], HIGH);
    digitalWrite(buttons_led[3], HIGH);

    currentStage = 0;                 // 스테이지 초기값 0으로 설정

    drawBackground_ready();           // 배경 그리기

    setupBlinkingText(10, "Stage 1");    // GLCD에 'Stage 1' 깜빡임 텍스트 설정
    setupLcdBlinkingText(readyText, 2);  // LCD에 2줄 깜빡임 텍스트 설정
}

// 게임 준비 씬 상태 업데이트 함수
void updateGameReadyScene() {
    if (digitalRead(buttons[0])) {            // Up 버튼 눌림 감지 (HIGH 상태일 때)
        if (currentStage > 0) {                // 첫 스테이지가 아니면
            currentStage--;                    // 스테이지 번호 감소

            setupBlinkingText(stageList[currentStage].line, stageList[currentStage].name); // 깜빡임 텍스트 업데이트

            delay(500);                        // 버튼 연속 입력 방지 딜레이
        }
    }
    else if (digitalRead(buttons[1])) {       // Down 버튼 눌림 감지 (HIGH 상태일 때)
        if (currentStage < stageNum - 1) {   // 마지막 스테이지가 아니면
            currentStage++;                    // 스테이지 번호 증가

            setupBlinkingText(stageList[currentStage].line, stageList[currentStage].name); // 깜빡임 텍스트 업데이트

            delay(500);                        // 버튼 연속 입력 방지 딜레이
        }
    }
    else if (digitalRead(buttons[2]) == LOW) { // Ok 버튼 눌림 감지 (LOW 활성화)
        changeScene(GAME_PLAY);                // 게임 플레이 씬으로 전환
        return;
    }
    else if (digitalRead(buttons[3]) == LOW) { // Exit 버튼 눌림 감지 (LOW 활성화)
        changeScene(START);                    // 시작 씬으로 전환
        return;
    }

    updateBlinkingText();                      // GLCD 깜빡임 텍스트 업데이트
    updateLcdBlinkingText(0);                  // LCD 깜빡임 텍스트 업데이트 (0줄)
}
