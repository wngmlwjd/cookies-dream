#include <openGLCD.h>
#include <LiquidCrystal.h>

#include "cookie.h"
#include "sceneManager.h"
#include "led.h"

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

// 0에서 9까지 숫자 표현을 위한
// 세그먼트 a, b, c, d, e, f, g, dp의 패턴
byte patterns[] = {
    0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6
};
int digit_select_pin[] = {66, 67, 68, 69}; // 자릿수 선택 핀
// 7세그먼트 모듈 연결 핀 ‘a, b, c, d, e, f, g, dp’ 순서
int segment_pin[] = {58, 59, 60, 61, 62, 63, 64, 65};

int RGB_LED[] = {8, 7, 6};

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
    
    for (int i = 0; i < 8; i++) {
        pinMode(segment_pin[i], OUTPUT);
        digitalWrite(segment_pin[i], LOW); // 초기값
    }

    for (int i = 0; i < 4; i++) {
        pinMode(digit_select_pin[i], OUTPUT);
        digitalWrite(digit_select_pin[i], HIGH); // HIGH면 OFF (공통 애노드 기준)
    }

    changeScene(START);

    for(int i = 0;i < 3;i++) {
        pinMode(RGB_LED[i], OUTPUT);    
    }

    setBestScore(0, 0);

    led_off();

    // Serial.println(read_eeprom_int(0));
}

void loop() {
    updateScene();
}