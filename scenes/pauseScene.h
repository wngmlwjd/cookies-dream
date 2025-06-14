#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include "../sceneManager.h"

extern int buttons[];
extern int buttons_led[];
extern LiquidCrystal lcd;
extern Scene currentScene;

void showPauseScene();
void updatePauseScene();

#endif