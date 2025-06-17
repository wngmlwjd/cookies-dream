#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

struct Obstacle {
    unsigned long spawnTime;
};

struct Stage {
    Obstacle* obstacles;
    int obstacleCount;
};

extern Stage stages[];
extern int currentStage;

extern const int cookieX;
extern int jumpY;

extern const int score_per_jump;
extern const int score_per_collision;

extern int gameScore;

void resetObstacles();
bool updateAndDrawObstacles(unsigned long currentTime);

bool isAllObstaclesCleared();
bool shouldSpawnFinalCookie();

bool checkCollisionWithCookie(double obsX, int obsY, int obsW, int obsH);

#endif
