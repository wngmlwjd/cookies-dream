#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

struct Obstacle {
    unsigned long spawnTime; // 생성 시간
};

struct Stage {
    Obstacle* obstacles; // 장애물 배열 포인터
    int obstacleCount; // 장애물 개수
};

extern Stage stages[]; // 스테이지 배열
extern int currentStage; // 현재 스테이지 인덱스

extern const int cookieX; // 쿠키 x좌표
extern int jumpY; // 점프 높이

extern const int score_per_jump; // 점프 시 점수
extern const int score_per_collision; // 충돌 시 점수 차감

extern int gameScore; // 게임 점수

void resetObstacles(); // 장애물 상태 초기화
bool updateAndDrawObstacles(unsigned long currentTime); // 장애물 업데이트 및 그리기

bool isAllObstaclesCleared(); // 모든 장애물 처리 완료 여부 확인
bool shouldSpawnFinalCookie(); // 마지막 쿠키 출현 조건 판단

bool checkCollisionWithCookie(double obsX, int obsY, int obsW, int obsH); // 충돌 체크

#endif
