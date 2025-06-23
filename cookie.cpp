#include <openGLCD.h>                 // GLCD 제어 라이브러리 포함

#include "cookie.h"                  // 쿠키 관련 함수 및 변수 선언 포함

// 쿠키의 고정 위치 좌표 선언 및 초기화
const int cookieX = 20;                // 쿠키 X 좌표
const int cookieY = 42;                // 쿠키 Y 좌표

// 점프 관련 변수 선언
int jumpHeight = 15;                   // 점프 최대 높이 (프레임 수)
const int jumpStep = 1;                // 점프 시 높이 증가 단위 (미사용)
int jumpDelay = 30;                    // 점프 딜레이 (프레임 당 시간 간격)
int jumpY = 0;                        // 현재 점프 높이 상태
int pre_elapsed;                      // 이전 점프 경과 시간 저장용
int now_elapsed;                      // 현재 점프 경과 시간 저장용
bool FIRST = true;                    // 최초 점프 여부 플래그

// 네모난 쿠키 그리기 함수 (크기 13x13)
void drawSquareCookie(int x, int y) {
    GLCD.DrawRect(x, y, 13, 13);           // 테두리 사각형 그리기

    GLCD.DrawRect(x + 3, y + 4, 2, 2);     // 왼쪽 눈
    GLCD.DrawRect(x + 8, y + 4, 2, 2);     // 오른쪽 눈
    GLCD.DrawRect(x + 4, y + 8, 5, 1);     // 입
}

// 웃는 표정 네모 쿠키 그리기 함수 (크기 13x13)
void drawSmileCookie(int x, int y) {
    GLCD.DrawRect(x, y, 13, 13);            // 테두리 사각형 그리기

    GLCD.DrawRect(x + 3, y + 4, 2, 2);      // 왼쪽 눈
    GLCD.DrawRect(x + 8, y + 4, 2, 2);      // 오른쪽 눈

    GLCD.DrawRect(x + 4, y + 8, 1, 1);      // 웃는 입 부분 픽셀들
    GLCD.DrawRect(x + 5, y + 9, 1, 1);
    GLCD.DrawRect(x + 6, y + 9, 1, 1);
    GLCD.DrawRect(x + 7, y + 9, 1, 1);
    GLCD.DrawRect(x + 8, y + 8, 1, 1);
}

// 동그란 쿠키 그리기 함수 (반지름 6, 13x13 크기 안에 그려짐)
void drawRoundCookie(int x, int y) {
    GLCD.DrawCircle(x + 6, y + 6, 6);       // 중심 (x+6, y+6), 반지름 6 원 그리기

    GLCD.DrawRect(x + 3, y + 4, 2, 2);      // 왼쪽 눈
    GLCD.DrawRect(x + 8, y + 4, 2, 2);      // 오른쪽 눈
    GLCD.DrawRect(x + 4, y + 8, 1, 1);      // 입 부분 픽셀들
    GLCD.DrawRect(x + 5, y + 9, 1, 1);
    GLCD.DrawRect(x + 6, y + 9, 1, 1);
    GLCD.DrawRect(x + 7, y + 9, 1, 1);
    GLCD.DrawRect(x + 8, y + 8, 1, 1);
}

// 점프 상태 업데이트 함수
// isJumping: 현재 점프 중인지 여부
// jumpStartTime: 점프 시작 시점 (밀리초 단위)
bool updateJump(bool isJumping, unsigned long jumpStartTime) {
    if (!isJumping) return isJumping;        // 점프 중이 아니면 바로 반환

    unsigned long now = millis();             // 현재 시간 (밀리초)
    int elapsed = (now - jumpStartTime);      // 점프 시작부터 경과 시간 계산
    int totalFrames = (jumpHeight * 2);       // 점프 전체 프레임 (상승 + 하강)
    int currentFrame = elapsed / jumpDelay;   // 현재 프레임 계산

    if (RESUME) {                             // 일시정지에서 재개 중일 때
        if (CHECK) {                          // 체크 플래그가 true 면 초기 설정
            CHECK = false;

            jumpStartTime_play = now;         // 점프 재개 시점 저장
            pre_elapsed = now_elapsed;        // 이전 경과 시간 저장
            elapsed = pre_elapsed;            // 경과 시간 초기화

            // Serial.println("CHECK");
            // Serial.println(elapsed);
            // Serial.println(jumpStartTime_play);
        }
        else {
            elapsed += pre_elapsed;            // 경과 시간에 이전 시간 누적
        }

        currentFrame = elapsed / jumpDelay;   // 재계산된 현재 프레임
    }

    now_elapsed = elapsed;                     // 현재 경과 시간 저장

    // Serial.println("Jump");
    // Serial.println(elapsed);
    // Serial.println(now);

    if (currentFrame >= totalFrames) {        // 점프 종료 조건
        isJumping = false;                     // 점프 종료
        jumpY = 0;                            // 점프 높이 초기화
        RESUME = false;                       // 재개 플래그 초기화
        FIRST = true;                        // 최초 점프 플래그 초기화

        return isJumping;                     // false 반환
    }

    // 점프 높이 설정: 상승과 하강 구간 나누기
    if (currentFrame < jumpHeight) {
        jumpY = currentFrame;                 // 상승 구간
    } else {
        jumpY = totalFrames - currentFrame;  // 하강 구간
    }

    return isJumping;                         // 점프 중 상태 반환 (true)
}
