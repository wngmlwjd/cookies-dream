#include <LiquidCrystal.h>
#include <openGLCD.h>

#include "../sceneManager.h"
#include "../cookie.h"
#include "../textUtils.h"

#include "gamePlayScene.h"

// 점프 상태 변수
bool isJumping_ready;
unsigned long jumpStartTime_ready = 0;

void drawBackground_ready() {
    GLCD.ClearScreen();

    drawGround();
    drawCenteredText(15, "Welcome!!");
}

void showGameReadyScene() {
    digitalWrite(buttons_led[0], HIGH);
    digitalWrite(buttons_led[2], HIGH);
    digitalWrite(buttons_led[3], HIGH);

    GLCD.ClearScreen();
    lcd.clear();
    lcd.print("High Score:");
    lcd.setCursor(0, 1);
    lcd.print("Jump|Start|Exit");

    jumpY = 0;
    jumpDelay = 50;
    jumpHeight = 10;
}

void updateGameReadyScene() {
    if (digitalRead(buttons[2]) == LOW) { // start
        changeScene(GAME_PLAY);

        return;
    }
    else if (digitalRead(buttons[3]) == LOW) { // exit
        isJumping_ready = false;
        changeScene(START);

        return;
    }

    int x = 50;

    drawBackground_ready();
    drawSquareCookie(x, cookieY - jumpY);
    drawRoundCookie(x + 15, cookieY - jumpY);

    if(!isJumping_ready) {
        isJumping_ready = !isJumping_ready;
        jumpStartTime_ready = millis();
    }

    isJumping_ready = updateJump(isJumping_ready, jumpStartTime_ready);
}
