#include <openGLCD.h>                        // GLCD 사용을 위한 라이브러리

#include "../sceneManager.h"                // 씬 전환 관련 함수 및 변수 포함
#include "../cookie.h"                      // 쿠키 관련 그래픽 함수
#include "../textUtils.h"                   // 텍스트 처리 함수
#include "../eeprom.h"                      // EEPROM 데이터 처리 함수

#include "endScene.h"                       // 현재 종료 씬 헤더

// 점프 상태 변수
bool isJumping_ending;                      // 종료 씬에서 점프 중 여부
unsigned long jumpStartTime_ending = 0;     // 종료 씬 점프 시작 시간

// 엔딩 씬 관련
double squareCookieX = 20;                  // 네모 쿠키의 초기 X 좌표
const double finalCookieX = 100;            // 동그란 쿠키의 고정 X 좌표
const int finalCookieY = 42;                // 동그란 쿠키의 Y 좌표
const double cookieSpeed = 0.5;             // 네모 쿠키의 이동 속도

// 게임 점수
int gameScore = 0;                          // 플레이어의 현재 점수

// 최고 기록 갱신 여부
bool NEW = false;                           // 새로운 최고 점수 달성 여부

// LCD 텍스트 출력용 구조체 배열
LcdTextLine endText[] = {
    {0, 0, "High Score: "}                  // 첫 줄에 출력할 내용
};

// 엔딩 씬 배경 그리기
void drawBackground_ending() {
    GLCD.ClearScreen();                     // 화면 초기화
    drawGround();                           // 땅 그리기
    drawCenteredText(20, "Stage Clear!!");  // 중앙에 축하 문구 출력
}

// 엔딩 씬에서 쿠키 두 마리 만나는 장면 연출
void updateFinalCookieMeeting() {
    drawBackground_ending();                // 배경 다시 그림

    squareCookieX += cookieSpeed;           // 네모 쿠키 이동

    drawSquareCookie(squareCookieX, finalCookieY);        // 네모 쿠키 출력
    drawRoundCookie(finalCookieX, finalCookieY);          // 동그란 쿠키 출력
}

// 종료 씬 진입 시 초기 설정
void showEndScene() {
    digitalWrite(buttons_led[3], HIGH);     // 'OK/종료' 버튼 LED 켜기

    drawBackground_ending();                // 배경 출력
    isJumping_ending = false;               // 점프 상태 초기화

    jumpY = 0;                              // 점프 높이 초기화
    jumpDelay = 100;                        // 점프 프레임 간 지연
    jumpHeight = 10;                        // 점프 최대 높이
    NEW = false;                            // 최고 기록 갱신 여부 초기화

    lcd.clear();                            // LCD 초기화
    lcd.print("High Score: ");              // 최고 점수 안내 출력
    lcd.setCursor(0, 1);                    // LCD 두 번째 줄로 이동
    lcd.print("Score: ");                   // 현재 점수 출력
    lcd.setCursor(12, 1);
    lcd.print("| OK");                      // 오른쪽에 OK 안내

    if (bestScore < gameScore) {            // 최고 점수 갱신 시
        Serial.println("NEW BEST SCORE");   // 디버깅 출력
        Serial.println(getBestScore(currentStage));
        setBestScore(currentStage, gameScore);             // EEPROM에 저장
        Serial.println(getBestScore(currentStage));

        bestScore = getBestScore(currentStage);            // 최신 최고 점수 반영

        endText[0].text = "High Score: " + String(bestScore);  // LCD 깜빡일 텍스트 업데이트

        setupLcdBlinkingText(endText, 1);                  // 깜빡이 텍스트 설정

        NEW = true;                                        // NEW 플래그 활성화
    }

    lcd.setCursor(12, 0);                  // 최고 점수 출력
    lcd.print(bestScore);
    lcd.setCursor(7, 1);                   // 현재 점수 출력
    lcd.print(gameScore);
}

// 종료 씬 업데이트 (loop에서 반복 호출)
void updateEndScene() {
    if (digitalRead(buttons[3]) == LOW) {  // '종료' 버튼 눌림 감지
        GAME_START = false;                // 게임 시작 상태 초기화
        isJumping_play = false;            // 점프 상태 초기화
        gameScore = 0;                     // 점수 초기화

        noTone(speakerPin);                // 사운드 정지

        clear_segments();                  // 7세그먼트 초기화
        
        changeScene(START);                // 시작 씬으로 전환
        return;
    }

    if (NEW) {
        updateLcdBlinkingText(0);          // 최고 점수 깜빡이 텍스트 업데이트
    }

    if (squareCookieX + 16 < finalCookieX) {
        updateFinalCookieMeeting();        // 네모 쿠키가 아직 도달하지 않음
    }
    else {
        if (digitalRead(buttons[3]) == LOW) {  // 종료 버튼 재확인
            changeScene(START);            // 시작 씬으로 이동
            return;
        }

        drawBackground_ending();           // 배경 그리기
        drawSmileCookie(squareCookieX, finalCookieY - jumpY);   // 네모 쿠키 웃는 얼굴
        drawRoundCookie(finalCookieX, finalCookieY - jumpY);    // 동그란 쿠키

        if (!isJumping_ending) {
            isJumping_ending = true;       // 점프 시작
            jumpStartTime_ending = millis(); // 시작 시간 기록
        }

        isJumping_ending = updateJump(isJumping_ending, jumpStartTime_ending);  // 점프 상태 갱신
    }
}
