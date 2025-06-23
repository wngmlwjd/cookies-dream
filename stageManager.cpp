#include <openGLCD.h>

#include "cookie.h"
#include "led.h"

#include "stageManager.h"

const int MAX_OBS = 10; // 최대 장애물 개수
const int obstacleY = 48; // 장애물 Y 위치
const int obstacleW = 3; // 장애물 너비
const int obstacleH = 7; // 장애물 높이
const double obstacleSpeed = 0.3; // 장애물 이동 속도

struct ActiveObstacle {
    double x; // 장애물 X 위치
    bool active; // 활성 상태
    bool collided; // 충돌 상태
};

ActiveObstacle activeObstacles[MAX_OBS]; // 활성 장애물 배열
int currentObstacleIndex = 0; // 현재 스폰할 장애물 인덱스

Stage stages[] = {
    // Stage 0 - Easy (간격 5000ms 이상)
    { new Obstacle[5]{ {1000}, {6000}, {11000}, {16000}, {21000} }, 5 },

    // Stage 1 - Medium (간격 5000~6000ms)
    { new Obstacle[7]{ {1000}, {6000}, {11500}, {17000}, {22500}, {28000}, {33500} }, 7 },

    // Stage 2 - Hard (간격 5000~5500ms)
    { new Obstacle[10]{ {1000}, {6000}, {11500}, {17000}, {22500}, {28000}, {33500}, {39000}, {44500}, {50000} }, 10 }
};

int currentStage = 0; // 현재 스테이지

unsigned long lastObstacleSpawnTime = 0; // 마지막 장애물 스폰 시간

bool obstaclePaused = false; // 장애물 일시정지 상태
unsigned long obstaclePauseStartTime = 0; // 일시정지 시작 시간
int pausedObstacleIndex = -1; // 일시정지된 장애물 인덱스
int obstacleBlinkCount = 0; // 깜빡임 횟수
bool blinkVisible = true; // 깜빡임 상태
const unsigned long blinkInterval = 200; // 깜빡임 간격(ms)
unsigned long lastBlinkTime = 0; // 마지막 깜빡임 시간


void resetObstacles() {
    for (int i = 0; i < MAX_OBS; i++) {
        activeObstacles[i].active = false; // 비활성화
        activeObstacles[i].x = 0; // 위치 초기화
        activeObstacles[i].collided = false; // 충돌 상태 초기화
    }
    currentObstacleIndex = 0; // 현재 인덱스 초기화
    lastObstacleSpawnTime = 0; // 마지막 스폰 시간 초기화
}

void spawnObstacle() {
    for (int i = 0; i < MAX_OBS; ++i) {
        if (!activeObstacles[i].active) {
            activeObstacles[i].x = 128; // 화면 오른쪽 끝 위치
            activeObstacles[i].active = true; // 활성화
            activeObstacles[i].collided = false; // 충돌 초기화
            break;
        }
    }
}

bool updateAndDrawObstacles(unsigned long currentTime) {
    Stage& stage = stages[currentStage]; // 현재 스테이지 참조
    bool isCollided = false; // 충돌 여부 초기화

    // 장애물 스폰 조건 체크
    if (currentObstacleIndex < stage.obstacleCount &&
        currentTime >= stage.obstacles[currentObstacleIndex].spawnTime) {
        spawnObstacle(); // 장애물 스폰

        if (currentObstacleIndex == stage.obstacleCount - 1) {
            lastObstacleSpawnTime = millis(); // 마지막 장애물 스폰 시간 기록
        }
        currentObstacleIndex++; // 다음 장애물 인덱스 증가
    }

    for (int i = 0; i < MAX_OBS; ++i) {
        if (!activeObstacles[i].active) continue; // 비활성 장애물 건너뜀

        // 깜빡임 처리 중인 장애물
        if (obstaclePaused && i == pausedObstacleIndex) {
            if (millis() - lastBlinkTime >= blinkInterval) {
                blinkVisible = !blinkVisible; // 깜빡임 상태 토글
                lastBlinkTime = millis();
                obstacleBlinkCount++; // 깜빡임 횟수 증가

                if (obstacleBlinkCount >= 6) {  // 깜빡임 종료 조건
                    activeObstacles[i].active = false; // 비활성화
                    obstaclePaused = false; // 일시정지 해제
                    pausedObstacleIndex = -1; // 인덱스 초기화
                    obstacleBlinkCount = 0; // 횟수 초기화
                    blinkVisible = true; // 상태 초기화
                }
            }

            if (blinkVisible) {
                GLCD.FillRect(activeObstacles[i].x, obstacleY, obstacleW, obstacleH); // 장애물 그리기
            }
            continue;  // 위치 유지하며 다른 업데이트는 건너뜀
        }

        // 장애물 위치 업데이트
        activeObstacles[i].x -= obstacleSpeed;

        // 점수 처리: 쿠키를 지나갔을 때
        if (activeObstacles[i].x + obstacleW < cookieX && !activeObstacles[i].collided) {
            gameScore += score_per_jump; // 점수 증가
            activeObstacles[i].collided = true; // 충돌 처리 (점수 중복 방지)

            led_on(GREEN); // 녹색 LED 켜기
        }

        // 화면 밖으로 나간 장애물 비활성화
        if (activeObstacles[i].x + obstacleW < 0) {
            activeObstacles[i].active = false;
            continue;
        }

        GLCD.FillRect(activeObstacles[i].x, obstacleY, obstacleW, obstacleH); // 장애물 그리기

        // 쿠키와 충돌 체크
        if (!activeObstacles[i].collided) {
            if (checkCollisionWithCookie(activeObstacles[i].x, obstacleY, obstacleW, obstacleH)) {
                activeObstacles[i].collided = true; // 충돌 표시
                isCollided = true; // 충돌 발생 플래그

                pausedObstacleIndex = i; // 충돌 장애물 인덱스 저장
                obstaclePaused = true; // 일시정지 상태
                obstacleBlinkCount = 0; // 깜빡임 초기화
                lastBlinkTime = millis(); // 시간 초기화
                gameScore -= score_per_collision; // 점수 차감

                led_on(RED); // 빨간 LED 켜기

                Serial.println("collision!"); // 시리얼 출력
            }
        }
    }

    return isCollided; // 충돌 여부 반환
}

bool isAllObstaclesCleared() {
    Stage& stage = stages[currentStage]; // 현재 스테이지 참조

    if (currentObstacleIndex < stage.obstacleCount) {
        return false; // 아직 스폰할 장애물 남음
    }

    // 활성화된 장애물 있는지 체크
    for (int i = 0; i < MAX_OBS; ++i) {
        if (activeObstacles[i].active) {
            return false; // 활성 장애물 존재
        }
    }

    return true; // 모든 장애물 사라짐
}

bool checkCollisionWithCookie(double obsX, int obsY, int obsW, int obsH) {
    int cookieW = 13; // 쿠키 너비
    int cookieH = 13; // 쿠키 높이
    int cookieTop = cookieY - jumpY; // 쿠키 위쪽 위치
    int cookieBottom = cookieTop + cookieH; // 쿠키 아래쪽 위치

    int obsLeft = obsX; // 장애물 왼쪽
    int obsRight = obsX + obsW; // 장애물 오른쪽
    int obsTop = obsY; // 장애물 위쪽
    int obsBottom = obsY + obsH; // 장애물 아래쪽

    int cookieLeft = cookieX; // 쿠키 왼쪽
    int cookieRight = cookieX + cookieW; // 쿠키 오른쪽

    bool isCollision =
        cookieLeft < obsRight &&
        cookieRight > obsLeft &&
        cookieTop < obsBottom &&
        cookieBottom > obsTop; // 충돌 조건

    return isCollision;
}
