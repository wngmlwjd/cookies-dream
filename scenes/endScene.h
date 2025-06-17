#ifndef ENDSCENE_H
#define ENDSCENE_H

#include "../sceneManager.h"

extern Scene currentScene;
extern int jumpY;

extern int currentStage;

extern int bestScore;

void showEndScene();
void updateEndScene();

int getBestScore(int n);

#endif