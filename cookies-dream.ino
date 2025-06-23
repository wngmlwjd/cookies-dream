#include <openGLCD.h>              // GLCD 제어 라이브러리 포함
#include <LiquidCrystal.h>         // LCD 제어 라이브러리 포함

#include "cookie.h"                // 쿠키 관련 헤더 포함
#include "sceneManager.h"          // 씬 관리 헤더 포함
#include "led.h"                   // LED 제어 헤더 포함

#include "scenes/startScene.cpp"   // 시작 씬 소스 포함
#include "scenes/gameReadyScene.cpp"   // 스테이지 선택 씬 소스 포함
#include "scenes/gamePlayScene.cpp"    // 게임 플레이 씬 소스 포함
#include "scenes/pauseScene.cpp"        // 일시정지 씬 소스 포함
#include "scenes/endScene.cpp"          // 종료 씬 소스 포함

// GLCD 화면 크기 설정
int left = 0;                      // 왼쪽 경계
int right = 128;                   // 오른쪽 경계
int bottom = 64;                   // 아래쪽 경계
int top = 0;                       // 위쪽 경계

// LCD 객체 생성, 핀 번호: RS, E, D4, D5, D6, D7 순서
LiquidCrystal lcd(44, 45, 46, 47, 48, 49);

// 버튼 핀 번호 배열
int buttons[] = {14, 15, 16, 17};
// 버튼 위쪽 LED 핀 번호 배열
int buttons_led[] = {2, 3, 4, 5};

// 숫자 0~9를 표시하기 위한 7세그먼트 + dp 패턴 배열
byte patterns[] = {
    0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6
};

// 7세그먼트 각 자릿수 선택 핀 번호 배열
int digit_select_pin[] = {66, 67, 68, 69};
// 7세그먼트 모듈 각 세그먼트 핀 번호 배열 (a, b, c, d, e, f, g, dp 순서)
int segment_pin[] = {58, 59, 60, 61, 62, 63, 64, 65};

// RGB LED 핀 번호 배열
int RGB_LED[] = {8, 7, 6};

void setup() {
    Serial.begin(9600);                // 시리얼 통신 9600bps로 시작

    // 버튼 핀을 입력 풀업 모드로 설정
    for (int i = 0; i < 4; i++) {
        pinMode(buttons[i], INPUT_PULLUP);
    }
    // 버튼 위쪽 LED 핀을 출력 모드로 설정
    for (int i = 0; i < 4; i++) {
        pinMode(buttons_led[i], OUTPUT);
    }

    GLCD.Init();                      // GLCD 초기화
    GLCD.SelectFont(System5x7);       // GLCD 폰트 선택

    lcd.begin(16, 2);                 // LCD 16x2 문자 크기로 초기화

    // 7세그먼트 모듈 세그먼트 핀 출력 및 초기 LOW 설정
    for (int i = 0; i < 8; i++) {
        pinMode(segment_pin[i], OUTPUT);
        digitalWrite(segment_pin[i], LOW);    // 초기 상태 LOW
    }

    // 7세그먼트 자릿수 선택 핀 출력 및 초기 HIGH 설정
    for (int i = 0; i < 4; i++) {
        pinMode(digit_select_pin[i], OUTPUT);
        digitalWrite(digit_select_pin[i], HIGH);  // HIGH는 OFF (공통 애노드 기준)
    }

    changeScene(START);               // 시작 씬으로 전환

    // RGB LED 핀을 출력 모드로 설정
    for (int i = 0; i < 3; i++) {
        pinMode(RGB_LED[i], OUTPUT);
    }

    setBestScore(0, 0);               // 스테이지 0의 최고 점수 초기화

    led_off();                       // LED 모두 끄기

    // Serial.println(read_eeprom_int(0)); // EEPROM 값 출력 (필요시 주석 해제)
}

void loop() {
    updateScene();                   // 현재 씬 업데이트
}
