#include <openGLCD.h>

#include "../cookie.h"
#include "../textUtils.h"
#include "../sceneManager.h"
#include "../bgm.h"
#include "../stageManager.h"
#include "../led.h"
#include "endScene.h"
#include "../eeprom.h"

#include "gamePlayScene.h"

bool GAME_START;
bool RESUME = false;
bool CHECK = true;

unsigned long lastScoreUpdate = 0;
const unsigned long scoreInterval = 5000;  // 초마다 점수 +1
int bestScore;

const int score_per_time = 5;
const int score_per_jump = 20;
const int score_per_collision = 10;

// 점프 상태 변수
bool isJumping_play;
unsigned long jumpStartTime_play;

unsigned long pausedStartTime;

// 버튼 상태 추적
bool prevJumpButtonState = LOW;

unsigned long gameStartTime = 0;
unsigned long TOTAL_GAME_TIME = 0;
unsigned long PRE_GAME_TIME = 0;

bool pre_collided = false;

void drawGround() {
    GLCD.FillRect(0, 55, 128, 10);
}

void drawBackground_play() {
    GLCD.ClearScreen();

    drawGround();
    GLCD.GotoXY(0, 0);
    GLCD.print(stageList[currentStage].name);
    drawRightAlignedText(0, "Score: " + String(gameScore));
}

void showGamePlayScene() {
    digitalWrite(buttons_led[0], HIGH);
    digitalWrite(buttons_led[3], HIGH);

    bestScore = getBestScore(currentStage);

    lcd.clear();
    lcd.print("High Score:");
    lcd.setCursor(12, 0);
    lcd.print(bestScore);
    lcd.setCursor(0, 1);
    lcd.print("Jump   |   Pause");

    // drawCenteredText(20, stageList[currentStage].name);

    if(!GAME_START) {
        GAME_START = true;
        RESUME = false;
        CHECK = true;

        gameScore = 0;

        isJumping_play = false;
        jumpStartTime_play = 0;

        jumpY = 0;
        jumpDelay = 70;
        jumpHeight = 30;

        resetObstacles();

        TOTAL_GAME_TIME = 0;
        PRE_GAME_TIME = 0;

        currentNoteIndex = 0;
        lastNoteTime = millis();
        isBGMPlaying = true;

        Serial.println("START");

        GLCD.ClearScreen();
        drawCenteredText(30, stageList[currentStage].name);

        led_on(GREEN);

        delay(500);
    }
    else {
        RESUME = true;
        
        jumpStartTime_play = millis();
        // jumpStartTime_play += millis() - pausedStartTime;

        Serial.println("RESUME");
        Serial.println(jumpStartTime_play - pausedStartTime);
        Serial.println(jumpStartTime_play);
    }

    gameStartTime = millis() + 500;
}

void updateGamePlayScene() {
    // Pause 버튼 처리
    if (digitalRead(buttons[3]) == LOW && GAME_START) {
        stopBGM();

        pausedStartTime = millis();
        CHECK = true;
        PRE_GAME_TIME = TOTAL_GAME_TIME;

        changeScene(PAUSE);
        Serial.println("PAUSE");
        Serial.println(pausedStartTime);
        return;
    }

    // 플레이 타임 계산
    unsigned long now_play = millis();

    TOTAL_GAME_TIME = PRE_GAME_TIME + (now_play - gameStartTime);

    show_4_digit(TOTAL_GAME_TIME / 1000);

    drawBackground_play();
    drawSquareCookie(cookieX, cookieY - jumpY);
    playBGM();

    // // 장애물 업데이트 및 충돌 여부 체크
    bool collided = updateAndDrawObstacles(TOTAL_GAME_TIME);

    // 모든 장애물 클리어 시 게임 종료 처리
    if (isAllObstaclesCleared()) {
        clear_segments();
        led_off();
        changeScene(END);
        return;
    }

    // 점프 버튼 상태
    bool currentJumpButtonState = digitalRead(buttons[0]);  // HIGH일 때 눌림

    isJumping_play = updateJump(isJumping_play, jumpStartTime_play);

    // Jump 트리거
    if (prevJumpButtonState == LOW && currentJumpButtonState == HIGH && !isJumping_play) {
        isJumping_play = true;
        jumpStartTime_play = millis();
    }
    prevJumpButtonState = currentJumpButtonState;

    unsigned long score_now = millis();
    if (score_now - lastScoreUpdate >= scoreInterval) {
        gameScore += score_per_time;
        lastScoreUpdate = score_now;
    }
}
