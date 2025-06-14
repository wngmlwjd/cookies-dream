#include <openGLCD.h>
#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <LiquidCrystal.h>

#include "cookie.h"
#include "sceneManager.h"

#include "scenes/startScene.cpp"
#include "scenes/gameReadyScene.cpp"
#include "scenes/gamePlayScene.cpp"
#include "scenes/pauseScene.cpp"
#include "scenes/endScene.cpp"

// GLCD 사이즈
int left = 0;
int right = 128;
int bottom = 64;
int top = 0;

// LCD
LiquidCrystal lcd(44,45,46,47,48,49);

// 버튼 핀 번호
int buttons[] = {14, 15, 16, 17};
// 버튼 위쪽 LED 핀 번호
int buttons_led[] = {2, 3, 4, 5};

void setup() {
    // 시리얼 통신 초기화
    Serial.begin(9600);

    // 버튼 연결
    for(int i = 0;i < 4;i++) {
        pinMode(buttons[i], INPUT_PULLUP);
    }
    // 버튼 위쪽 LED 연결
    for(int i = 0;i < 4;i++) {
        pinMode(buttons_led[i], OUTPUT);
    }

    // GLCD 초기화
    GLCD.Init();
    GLCD.SelectFont(System5x7);

    // LCD 초기화
    lcd.begin(16, 2);

    changeScene(START);
}

void loop() {
    updateScene();

    // GLCD.ClearScreen();

    // drawSquareCookie(50, 40);
    // // drawRoundCookie(50, 40);

    // if(digitalRead(buttons[0])) {
    //     jump();
    // }

    // playBGM();

    // while(true);
}
