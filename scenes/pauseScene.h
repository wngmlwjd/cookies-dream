#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include "../sceneManager.h"

extern int buttons[];
extern int buttons_led[];
extern LiquidCrystal lcd;
extern Scene currentScene;

extern int speakerPin;

extern bool GAME_START;
extern bool isJumping_play;

void showPauseScene();
void updatePauseScene();

#endif