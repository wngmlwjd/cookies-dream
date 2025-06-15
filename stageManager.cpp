#include <openGLCD.h>

#include "stageManager.h"
#include "cookie.h"

const int MAX_OBS = 10;
const int obstacleY = 48;
const int obstacleW = 3;
const int obstacleH = 7;
const double obstacleSpeed = 0.3;

struct ActiveObstacle {
    double x;
    bool active;
};

ActiveObstacle activeObstacles[MAX_OBS];
int currentObstacleIndex = 0;

Stage stages[] = {
    // Stage 0 - Easy (간격 5000ms 이상)
    { new Obstacle[5]{ {1000}, {6000}, {11000}, {16000}, {21000} }, 5 },

    // Stage 1 - Medium (간격 5000~6000ms)
    { new Obstacle[7]{ {1000}, {6000}, {11500}, {17000}, {22500}, {28000}, {33500} }, 7 },

    // Stage 2 - Hard (간격 5000~5500ms)
    { new Obstacle[10]{ {1000}, {6000}, {11500}, {17000}, {22500}, {28000}, {33500}, {39000}, {44500}, {50000} }, 10 }
};

int currentStage = 0;

unsigned long lastObstacleSpawnTime = 0;

void resetObstacles() {
    for (int i = 0; i < MAX_OBS; i++) {
        activeObstacles[i].active = false;
        activeObstacles[i].x = 0;
    }
    currentObstacleIndex = 0;
    lastObstacleSpawnTime = 0;
}


void spawnObstacle() {
    for (int i = 0; i < MAX_OBS; ++i) {
        if (!activeObstacles[i].active) {
            activeObstacles[i].x = 128;
            activeObstacles[i].active = true;
            break;
        }
    }
}

void updateAndDrawObstacles(unsigned long currentTime) {
    Stage& stage = stages[currentStage];

    // 스폰 시간 도달 시 생성
    if (currentObstacleIndex < stage.obstacleCount &&
        currentTime >= stage.obstacles[currentObstacleIndex].spawnTime) {
        spawnObstacle();

        // 마지막 장애물이면 시간 기록
        if (currentObstacleIndex == stage.obstacleCount - 1) {
            lastObstacleSpawnTime = millis(); // 전체 시간 기준
        }
        currentObstacleIndex++;
    }

    // 이동 및 그리기
    for (int i = 0; i < MAX_OBS; ++i) {
        if (activeObstacles[i].active) {
            activeObstacles[i].x -= obstacleSpeed;
            if (activeObstacles[i].x + obstacleW < 0) {
                activeObstacles[i].active = false;
            } else {
                GLCD.FillRect(activeObstacles[i].x, obstacleY, obstacleW, obstacleH);
            }
        }
    }
}

bool isAllObstaclesCleared() {
    // 모든 장애물이 스폰되고, activeObstacles 배열에 활성화된 장애물이 하나도 없으면 true 반환
    Stage& stage = stages[currentStage];

    if (currentObstacleIndex < stage.obstacleCount) {
        // 아직 스폰할 장애물이 남아 있음
        return false;
    }

    // 활성화된 장애물이 있는지 확인
    for (int i = 0; i < MAX_OBS; ++i) {
        if (activeObstacles[i].active) {
            return false;
        }
    }

    return true; // 모든 장애물이 사라짐
}

bool shouldSpawnFinalCookie() {
    Stage& stage = stages[currentStage];

    return (currentObstacleIndex >= stage.obstacleCount &&
        isAllObstaclesCleared() &&
        millis() - lastObstacleSpawnTime >= 5000
    );
}
