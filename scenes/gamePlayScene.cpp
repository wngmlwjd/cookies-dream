#include <openGLCD.h>

#include "../cookie.h"
#include "../textUtils.h"
#include "../sceneManager.h"
#include "../bgm.h"

bool GAME_START;
bool RESUME = false;

// 점프 상태 변수
bool isJumping_play;
unsigned long jumpStartTime_play;

unsigned long pausedStartTime;

// 버튼 상태 추적
bool prevJumpButtonState = LOW;

void drawGround() {
    GLCD.FillRect(0, 55, 128, 10);
}

void drawBackground_play() {
    GLCD.ClearScreen();
    drawGround();
}

void showGamePlayScene() {
    digitalWrite(buttons_led[0], HIGH);
    digitalWrite(buttons_led[3], HIGH);

    drawBackground_play();

    lcd.clear();
    lcd.print("High Score:");
    lcd.setCursor(0, 1);
    lcd.print("Jump   |   Pause");

    drawCenteredText(20, "gameScene");

    if(!GAME_START) {
        GAME_START = true;
        RESUME = false;

        isJumping_play = false;
        jumpStartTime_play = 0;

        jumpY = 0;
        jumpDelay = 30;
        jumpHeight = 15;

        Serial.println("START");
    }
    else {
        RESUME = true;
        jumpStartTime_play = millis();
        // jumpStartTime_play += millis() - pausedStartTime;

        Serial.println("RESUME");
        Serial.println(jumpStartTime_play - pausedStartTime);
        Serial.println(jumpStartTime_play);
    }
}

void updateGamePlayScene() {
    // Pause (buttons[3]) - LOW일 때 눌림 (게임 중)
    if (digitalRead(buttons[3]) == LOW && GAME_START) {
        stopBGM();

        pausedStartTime = millis();

        Serial.println("PAUSE");
        Serial.println(pausedStartTime);
        return;
    }

    // 점프 버튼 처리
    bool currentJumpButtonState = digitalRead(buttons[0]);  // HIGH일 때 눌림

    isJumping_play = updateJump(isJumping_play, jumpStartTime_play);
    drawBackground_play();
    drawSquareCookie(cookieX, cookieY - jumpY);

    // 배경음악 계속 재생
    playBGM();

    // Jump 트리거
    if (prevJumpButtonState == LOW && currentJumpButtonState == HIGH && !isJumping_play) {
        isJumping_play = true;
        jumpStartTime_play = millis();
    }
    prevJumpButtonState = currentJumpButtonState;
}
