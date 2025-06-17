#include <openGLCD.h>

#include "cookie.h"

// 쿠키의 고정 위치
const int cookieX = 20;
const int cookieY = 42;

// 점프
int jumpHeight = 15;
const int jumpStep = 1;
int jumpDelay = 30;
int jumpY = 0;
int pre_elapsed;
int now_elapsed;
bool FIRST = true;

// 네모난 쿠키: 13x13 크기
void drawSquareCookie(int x, int y) {
    // 테두리 사각형 (13x13)
    GLCD.DrawRect(x, y, 13, 13);

    GLCD.DrawRect(x + 3, y + 4, 2, 2);
    GLCD.DrawRect(x + 8, y + 4, 2, 2);
    GLCD.DrawRect(x + 4, y + 8, 5, 1);
}
void drawSmileCookie(int x, int y) {
    // 테두리 사각형 (13x13)
    GLCD.DrawRect(x, y, 13, 13);

    GLCD.DrawRect(x + 3, y + 4, 2, 2);
    GLCD.DrawRect(x + 8, y + 4, 2, 2);

    GLCD.DrawRect(x + 4, y + 8, 1, 1);
    GLCD.DrawRect(x + 5, y + 9, 1, 1);
    GLCD.DrawRect(x + 6, y + 9, 1, 1);
    GLCD.DrawRect(x + 7, y + 9, 1, 1);
    GLCD.DrawRect(x + 8, y + 8, 1, 1);
}

// 동그란 쿠키: 반지름 6 (13x13 크기 안에)
void drawRoundCookie(int x, int y) {
    // 중심 (x+6, y+6), 반지름 6
    GLCD.DrawCircle(x + 6, y + 6, 6);

    GLCD.DrawRect(x + 3, y + 4, 2, 2);
    GLCD.DrawRect(x + 8, y + 4, 2, 2);
    GLCD.DrawRect(x + 4, y + 8, 1, 1);
    GLCD.DrawRect(x + 5, y + 9, 1, 1);
    GLCD.DrawRect(x + 6, y + 9, 1, 1);
    GLCD.DrawRect(x + 7, y + 9, 1, 1);
    GLCD.DrawRect(x + 8, y + 8, 1, 1);
}

bool updateJump(bool isJumping, unsigned long jumpStartTime) {
    if (!isJumping) return;

    unsigned long now = millis();
    int elapsed = (now - jumpStartTime);
    int totalFrames = (jumpHeight * 2);
    int currentFrame = elapsed / jumpDelay;

    if(RESUME) {
        if(CHECK) {
            CHECK = false;

            jumpStartTime_play = now;
            pre_elapsed = now_elapsed;
            elapsed = pre_elapsed;

            // Serial.println("CHECK");
            // Serial.println(elapsed);
            // Serial.println(jumpStartTime_play);
        }
        else {
            elapsed += pre_elapsed;
        }
        
        currentFrame = elapsed / jumpDelay;
    }

    now_elapsed = elapsed;

    // Serial.println("Jump");
    // Serial.println(elapsed);
    // Serial.println(now);

    if (currentFrame >= totalFrames) {
        isJumping = false;
        jumpY = 0;
        RESUME = false;
        FIRST = true;

        return isJumping;
    }

    if (currentFrame < jumpHeight) {
        jumpY = currentFrame;
    } else {
        jumpY = totalFrames - currentFrame;
    }

    return isJumping;
}
