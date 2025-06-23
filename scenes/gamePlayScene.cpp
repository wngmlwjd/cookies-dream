#include <openGLCD.h>                       // GLCD 제어 라이브러리 포함

#include "../cookie.h"                      // 쿠키 관련 함수 및 변수 포함
#include "../textUtils.h"                   // 텍스트 유틸리티 함수 포함
#include "../sceneManager.h"                // 씬 관리자 헤더 포함
#include "../bgm.h"                        // 배경음악 관련 함수 포함
#include "../stageManager.h"                // 스테이지 관리자 헤더 포함
#include "../led.h"                        // LED 제어 함수 포함
#include "endScene.h"                      // 종료 씬 헤더 포함
#include "../eeprom.h"                     // EEPROM 관련 함수 포함

#include "gamePlayScene.h"                  // 게임 플레이 씬 헤더 포함

bool GAME_START;                         // 게임 시작 여부
bool RESUME = false;                     // 게임 재개 상태 플래그
bool CHECK = true;                       // 점프 재개 체크용 플래그

unsigned long lastScoreUpdate = 0;      // 점수 갱신 시간 추적
const unsigned long scoreInterval = 5000;  // 점수 갱신 간격 (밀리초 단위, 5초마다 점수 증가)
int bestScore;                          // 현재 스테이지 최고 점수 저장 변수

const int score_per_time = 5;           // 시간당 점수 증가량
const int score_per_jump = 20;          // 점프 성공 시 점수
const int score_per_collision = 10;     // 장애물 충돌 시 점수

// 점프 상태 변수
bool isJumping_play;                    // 점프 중 여부
unsigned long jumpStartTime_play;       // 점프 시작 시간

unsigned long pausedStartTime;           // 일시정지 시작 시간

// 버튼 상태 추적 변수
bool prevJumpButtonState = LOW;          // 이전 점프 버튼 상태

unsigned long gameStartTime = 0;         // 게임 시작 시간
unsigned long TOTAL_GAME_TIME = 0;       // 총 플레이 시간
unsigned long PRE_GAME_TIME = 0;         // 이전 플레이 누적 시간 (일시정지 시 저장)

bool pre_collided = false;                // 이전 충돌 상태

// 땅 그리기 함수
void drawGround() {
    GLCD.FillRect(0, 55, 128, 10);        // 화면 아래쪽에 직사각형으로 땅 표현
}

// 게임 플레이 씬 배경 그리기 함수
void drawBackground_play() {
    GLCD.ClearScreen();                    // 화면 클리어

    drawGround();                         // 땅 그리기
    GLCD.GotoXY(0, 0);                    // 좌측 상단으로 커서 이동
    GLCD.print(stageList[currentStage].name);  // 현재 스테이지 이름 출력
    drawRightAlignedText(0, "Score: " + String(gameScore));  // 점수 우측 정렬 출력
}

// 게임 플레이 씬 초기화 및 화면 출력 함수
void showGamePlayScene() {
    digitalWrite(buttons_led[0], HIGH);   // 점프 버튼 LED ON
    digitalWrite(buttons_led[3], HIGH);   // 일시정지 버튼 LED ON

    bestScore = getBestScore(currentStage);   // EEPROM에서 최고 점수 불러오기

    lcd.clear();                         // LCD 초기화
    lcd.print("High Score:");            // LCD 첫 줄에 "High Score:" 출력
    lcd.setCursor(12, 0);                // 첫 줄 12번째 칸으로 커서 이동
    lcd.print(bestScore);                // 최고 점수 출력
    lcd.setCursor(0, 1);                 // 두 번째 줄 첫 칸으로 커서 이동
    lcd.print("Jump   |   Pause");       // 점프 및 일시정지 버튼 안내

    // 게임이 처음 시작되는 경우 초기화
    if (!GAME_START) {
        GAME_START = true;
        RESUME = false;
        CHECK = true;

        gameScore = 0;                   // 점수 초기화

        isJumping_play = false;          // 점프 상태 초기화
        jumpStartTime_play = 0;

        jumpY = 0;                      // 점프 높이 초기화
        jumpDelay = 70;                 // 점프 딜레이 초기값 설정
        jumpHeight = 30;                // 최대 점프 높이 설정

        resetObstacles();               // 장애물 초기화 함수 호출 (외부 정의 필요)

        TOTAL_GAME_TIME = 0;            // 총 게임 시간 초기화
        PRE_GAME_TIME = 0;              // 누적 시간 초기화

        currentNoteIndex = 0;           // 배경음악 초기화
        lastNoteTime = millis();
        isBGMPlaying = true;            // 배경음악 재생 상태 ON

        Serial.println("START");        // 시리얼 모니터에 시작 알림

        GLCD.ClearScreen();             // 화면 초기화
        drawCenteredText(30, stageList[currentStage].name);  // 중앙에 스테이지 이름 출력

        led_on(GREEN);                  // 녹색 LED ON

        delay(500);                    // 0.5초 대기
    }
    else {                             // 게임 재개 처리
        RESUME = true;
        
        jumpStartTime_play = millis(); // 점프 시작 시간 재설정 (게임 재개 시점)

        // Serial 출력 디버깅용
        Serial.println("RESUME");
        Serial.println(jumpStartTime_play - pausedStartTime);
        Serial.println(jumpStartTime_play);
    }

    gameStartTime = millis() + 500;    // 게임 시작 시간 설정 (0.5초 지연 포함)
}

// 게임 플레이 씬 상태 업데이트 함수
void updateGamePlayScene() {
    // 일시정지 버튼 처리
    if (digitalRead(buttons[3]) == LOW && GAME_START) {  // 일시정지 버튼 눌림 감지
        stopBGM();                    // 배경음악 정지

        pausedStartTime = millis();   // 일시정지 시작 시간 저장
        CHECK = true;                 // 점프 재개 체크 플래그 활성화
        PRE_GAME_TIME = TOTAL_GAME_TIME;  // 현재 누적 게임 시간 저장

        changeScene(PAUSE);           // 씬을 일시정지 씬으로 변경
        Serial.println("PAUSE");      // 디버깅 출력
        Serial.println(pausedStartTime);
        return;                      // 이후 코드 실행 중단
    }

    // 현재 시간 기준 플레이 시간 계산
    unsigned long now_play = millis();
    TOTAL_GAME_TIME = PRE_GAME_TIME + (now_play - gameStartTime);

    show_4_digit(TOTAL_GAME_TIME / 1000);  // 7세그먼트에 현재 시간(초) 표시

    drawBackground_play();                // 배경 그리기
    drawSquareCookie(cookieX, cookieY - jumpY);  // 쿠키 그리기 (점프 높이 반영)
    playBGM();                           // 배경음악 재생 처리

    // 장애물 업데이트 및 충돌 체크
    bool collided = updateAndDrawObstacles(TOTAL_GAME_TIME);

    // 모든 장애물 클리어 시 게임 종료 처리
    if (isAllObstaclesCleared()) {
        clear_segments();                // 7세그먼트 초기화
        led_off();                      // LED 모두 끄기
        changeScene(END);               // 종료 씬으로 전환
        return;
    }

    // 점프 버튼 상태 읽기 (HIGH: 눌림)
    bool currentJumpButtonState = digitalRead(buttons[0]);

    // 점프 상태 업데이트
    isJumping_play = updateJump(isJumping_play, jumpStartTime_play);

    // 점프 트리거 감지: 이전에는 안 눌림(LOW), 현재 눌림(HIGH), 점프 중 아님
    if (prevJumpButtonState == LOW && currentJumpButtonState == HIGH && !isJumping_play) {
        isJumping_play = true;             // 점프 시작
        jumpStartTime_play = millis();     // 점프 시작 시간 기록
    }
    prevJumpButtonState = currentJumpButtonState; // 이전 점프 버튼 상태 갱신

    // 점수 갱신 처리 (5초마다 점수 증가)
    unsigned long score_now = millis();
    if (score_now - lastScoreUpdate >= scoreInterval) {
        gameScore += score_per_time;       // 시간 경과 점수 증가
        lastScoreUpdate = score_now;       // 점수 갱신 시간 업데이트
    }
}
