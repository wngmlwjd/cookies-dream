#ifndef GAMEPLAYSCENE_H               // 헤더 파일 중복 포함 방지 시작
#define GAMEPLAYSCENE_H               // GAMEPLAYSCENE_H 매크로 정의

#include "gameReadyScene.h"          // 게임 준비 씬 헤더 포함

// 메인 관련 외부 변수 선언
extern LiquidCrystal lcd;           // LCD 객체 외부 선언
extern int buttons[4];              // 버튼 핀 배열 외부 선언
extern int buttons_led[4];          // 버튼 LED 핀 배열 외부 선언

// 쿠키 관련 외부 변수 선언
extern int jumpY;                   // 점프 높이 상태
extern unsigned long jumpStartTime;// 점프 시작 시간 (밀리초)
extern int jumpDelay;               // 점프 딜레이
extern int jumpHeight;              // 점프 최대 높이
extern int elapsed;                 // 점프 경과 시간 또는 프레임 수

// 배경음악 및 게임 진행 변수
extern int currentNoteIndex;        // 현재 재생 음표 인덱스
extern unsigned long lastNoteTime;  // 마지막 음표 재생 시간
extern bool isBGMPlaying;            // 배경음악 재생 상태
extern int currentStage;             // 현재 선택된 스테이지 번호

extern bool finalCookieSpawned;           // 마지막 쿠키 출현 여부
extern unsigned long lastObstacleSpawnTime;  // 마지막 장애물 출현 시간

extern int pre_elapsed;              // 이전 경과 시간
extern int now_elapsed;              // 현재 경과 시간

extern int gameScore;                // 게임 점수

extern stageName stageList[];       // 스테이지 이름 리스트

// 게임 플레이 씬 관련 함수 선언
void drawGround();                  // 땅 그리기 함수
void drawBackground_play();         // 게임 플레이 배경 그리기 함수
void updateJump();                  // 점프 상태 업데이트 함수

void showGamePlayScene();           // 게임 플레이 씬 화면 출력 함수
void updateGamePlayScene();         // 게임 플레이 씬 상태 업데이트 함수

// 마지막 쿠키와의 만남 관련 함수
void updateFinalCookieMeeting();    // 마지막 쿠키 만남 상태 업데이트
bool isFinalMeetingStarted();       // 마지막 만남 시작 여부 확인
void startFinalMeeting();           // 마지막 만남 시작 함수

#endif                             // 헤더 파일 중복 포함 방지 끝
