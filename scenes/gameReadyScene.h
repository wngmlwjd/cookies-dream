#ifndef GAMEREADYSCENE_H              // 헤더 파일 중복 포함 방지 시작
#define GAMEREADYSCENE_H              // GAMEREADYSCENE_H 매크로 정의

// 스테이지 이름과 위치 정보를 담는 구조체 선언
struct stageName {
    int line;                       // LCD 또는 화면의 줄 번호
    String name;                    // 스테이지 이름 문자열
};

// 메인 관련 외부 변수 선언
extern LiquidCrystal lcd;          // LCD 객체 외부 선언
extern int buttons[4];             // 버튼 핀 배열 외부 선언
extern int buttons_led[4];         // 버튼 LED 핀 배열 외부 선언

// 쿠키 관련 외부 변수 선언
extern int jumpY;                  // 현재 점프 높이 상태
extern int jumpDelay;              // 점프 딜레이
extern int jumpHeight;             // 점프 최대 높이

extern int currentStage;           // 현재 선택된 스테이지 번호

// 게임 준비 씬 관련 함수 선언
void showGameReadyScene();         // 게임 준비(스테이지 선택) 씬 화면 출력 함수
void updateGameReadyScene();       // 게임 준비 씬 상태 업데이트 함수

void drawBackground_ready();       // 게임 준비 씬 배경 그리기 함수

#endif                            // 헤더 파일 중복 포함 방지 끝
