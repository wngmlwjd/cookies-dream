#include <openGLCD.h>
#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>

#include "cookie.h"

// 쿠키의 고정 위치
const int cookieX = 20;
const int cookieY = 42;

// 네모난 쿠키: 13x13 크기
void drawSquareCookie(int x, int y) {
    // 테두리 사각형 (13x13)
    GLCD.DrawRect(x, y, 13, 13);

    GLCD.DrawRect(x + 3, y + 4, 2, 2);
    GLCD.DrawRect(x + 8, y + 4, 2, 2);
    GLCD.DrawRect(x + 4, y + 8, 5, 1);
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
