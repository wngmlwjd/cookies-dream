#include <openGLCD.h>

#include "cookie.h"
#include "led.h"

#include "stageManager.h"

const int MAX_OBS = 10;
const int obstacleY = 48;
const int obstacleW = 3;
const int obstacleH = 7;
const double obstacleSpeed = 0.3;

struct ActiveObstacle {
    double x;
    bool active;
    bool collided;
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

bool obstaclePaused = false;
unsigned long obstaclePauseStartTime = 0;
int pausedObstacleIndex = -1;
int obstacleBlinkCount = 0;
bool blinkVisible = true;
const unsigned long blinkInterval = 200;
unsigned long lastBlinkTime = 0;


void resetObstacles() {
    for (int i = 0; i < MAX_OBS; i++) {
        activeObstacles[i].active = false;
        activeObstacles[i].x = 0;
        activeObstacles[i].collided = false; 
    }
    currentObstacleIndex = 0;
    lastObstacleSpawnTime = 0;
}

void spawnObstacle() {
    for (int i = 0; i < MAX_OBS; ++i) {
        if (!activeObstacles[i].active) {
            activeObstacles[i].x = 128;
            activeObstacles[i].active = true;
            activeObstacles[i].collided = false;
            break;
        }
    }
}

bool updateAndDrawObstacles(unsigned long currentTime) {
    Stage& stage = stages[currentStage];
    bool isCollided = false;

    // 장애물 스폰
    if (currentObstacleIndex < stage.obstacleCount &&
        currentTime >= stage.obstacles[currentObstacleIndex].spawnTime) {
        spawnObstacle();

        if (currentObstacleIndex == stage.obstacleCount - 1) {
            lastObstacleSpawnTime = millis();
        }
        currentObstacleIndex++;
    }

    for (int i = 0; i < MAX_OBS; ++i) {
        if (!activeObstacles[i].active) continue;

        // 깜빡이는 장애물 처리
        if (obstaclePaused && i == pausedObstacleIndex) {
            if (millis() - lastBlinkTime >= blinkInterval) {
                blinkVisible = !blinkVisible;
                lastBlinkTime = millis();
                obstacleBlinkCount++;

                if (obstacleBlinkCount >= 6) {  // 깜빡임 끝
                    activeObstacles[i].active = false;
                    obstaclePaused = false;
                    pausedObstacleIndex = -1;
                    obstacleBlinkCount = 0;
                    blinkVisible = true;
                }
            }

            if (blinkVisible) {
                GLCD.FillRect(activeObstacles[i].x, obstacleY, obstacleW, obstacleH);
            }
            continue;  // 다른 처리 생략 (위치는 유지됨)
        }

        // 일반 장애물 업데이트
        activeObstacles[i].x -= obstacleSpeed;

        // 점수 처리
        if (activeObstacles[i].x + obstacleW < cookieX && !activeObstacles[i].collided) {
            gameScore += score_per_jump;
            activeObstacles[i].collided = true;

            led_on(GREEN);
        }

        // 화면 밖 제거
        if (activeObstacles[i].x + obstacleW < 0) {
            activeObstacles[i].active = false;
            continue;
        }

        GLCD.FillRect(activeObstacles[i].x, obstacleY, obstacleW, obstacleH);

        // 충돌 처리
        if(!activeObstacles[i].collided) {
            if(checkCollisionWithCookie(activeObstacles[i].x, obstacleY, obstacleW, obstacleH)) {
                activeObstacles[i].collided = true;
                isCollided = true;

                pausedObstacleIndex = i;
                obstaclePaused = true;
                obstacleBlinkCount = 0;
                lastBlinkTime = millis();
                gameScore -= score_per_collision;

                led_on(RED);

                Serial.println("collision!");
            }
        }
    }

    return isCollided;
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

bool checkCollisionWithCookie(double obsX, int obsY, int obsW, int obsH) {
    int cookieW = 13;
    int cookieH = 13;
    int cookieTop = cookieY - jumpY;
    int cookieBottom = cookieTop + cookieH;

    int obsLeft = obsX;
    int obsRight = obsX + obsW;
    int obsTop = obsY;
    int obsBottom = obsY + obsH;

    int cookieLeft = cookieX;
    int cookieRight = cookieX + cookieW;

    bool isCollision =
        cookieLeft < obsRight &&
        cookieRight > obsLeft &&
        cookieTop < obsBottom &&
        cookieBottom > obsTop;

    return isCollision;
}
