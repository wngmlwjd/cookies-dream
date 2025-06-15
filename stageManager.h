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

void resetObstacles();
void updateAndDrawObstacles(unsigned long currentTime);

bool isAllObstaclesCleared();
bool shouldSpawnFinalCookie();

#endif
