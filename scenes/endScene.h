#ifndef ENDSCENE_H               // 헤더 파일 중복 포함 방지 시작
#define ENDSCENE_H               // ENDSCENE_H 매크로 정의

#include "../sceneManager.h"    // 씬 매니저 헤더 포함

extern Scene currentScene;      // 현재 씬 변수 외부 선언
extern int jumpY;               // 쿠키 점프 높이 외부 선언

extern int currentStage;        // 현재 스테이지 번호 외부 선언

extern int bestScore;           // 최고 점수 외부 선언

// 종료 씬 관련 함수 선언
void showEndScene();            // 종료 씬 화면 출력 함수
void updateEndScene();          // 종료 씬 상태 업데이트 함수

int getBestScore(int n);        // 특정 스테이지의 최고 점수 반환 함수

#endif                         // 헤더 파일 중복 포함 방지 끝
