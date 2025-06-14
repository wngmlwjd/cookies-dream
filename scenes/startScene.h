#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "../sceneManager.h"

extern int buttons[];
extern int buttons_led[];
extern LiquidCrystal lcd;
extern Scene currentScene;

void showStartScene();
void updateStartScene();

#endif