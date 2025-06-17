#include <openGLCD.h>

#include "../sceneManager.h"
#include "../cookie.h"
#include "../textUtils.h"
#include "../eeprom.h"

#include "endScene.h"

// 점프 상태 변수
bool isJumping_ending;
unsigned long jumpStartTime_ending = 0;

// 엔딩 씬 관련
double squareCookieX = 20;  // 출발 위치
const double finalCookieX = 100;
const int finalCookieY = 42;
const double cookieSpeed = 0.5; // 네모 쿠키 이동 속도

// 게임 점수
int gameScore = 0;
// 최고 기록 갱신 여부
bool NEW = false;

// LCD
LcdTextLine endText[] = {
    {0, 0, "High Score: "}
};

// 엔딩 씬
void drawBackground_ending() {
    GLCD.ClearScreen();
    drawGround();
    drawCenteredText(20, "Stage Clear!!");
}
void updateFinalCookieMeeting() {
    drawBackground_ending();

    squareCookieX += cookieSpeed;

    drawSquareCookie(squareCookieX, finalCookieY);
    drawRoundCookie(finalCookieX, finalCookieY);
}

void showEndScene() {
    digitalWrite(buttons_led[3], HIGH);

    drawBackground_ending();
    isJumping_ending = false;

    jumpY = 0;
    jumpDelay = 100;
    jumpHeight = 10;
    NEW = false;

    lcd.clear();
    lcd.print("High Score: ");
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.setCursor(12, 1);
    lcd.print("| OK");

    if(bestScore < gameScore) {
        Serial.println("NEW BEST SCORE");
        Serial.println(getBestScore(currentStage));
        setBestScore(currentStage, gameScore);
        Serial.println(getBestScore(currentStage));

        bestScore = getBestScore(currentStage);

        endText[0].text = "High Score: " + String(bestScore);

        setupLcdBlinkingText(endText, 1);

        NEW = true;
    }

    lcd.setCursor(12, 0);
    lcd.print(bestScore);
    lcd.setCursor(7, 1);
    lcd.print(gameScore);
}

void updateEndScene() {
    if(digitalRead(buttons[3]) == LOW) {
        // 종료 → 모든 상태 초기화 필요
        GAME_START = false;
        isJumping_play = false;
        gameScore = 0;

        noTone(speakerPin);

        clear_segments();
        
        changeScene(START);

        return;
    }

    if(NEW) {
        updateLcdBlinkingText(0); // LCD
    }

    if(squareCookieX + 16 < finalCookieX) {
        updateFinalCookieMeeting();
    }
    else {
        if (digitalRead(buttons[3]) == LOW) { // exit
            changeScene(START);

            return;
        }

        drawBackground_ending();
        drawSmileCookie(squareCookieX, finalCookieY - jumpY);
        drawRoundCookie(finalCookieX, finalCookieY - jumpY);

        if(!isJumping_ending) {
            isJumping_ending = !isJumping_ending;
            jumpStartTime_ending = millis();
        }

        isJumping_ending = updateJump(isJumping_ending, jumpStartTime_ending);
    }
}