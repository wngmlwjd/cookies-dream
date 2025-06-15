#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <LiquidCrystal.h>

enum Scene {
  START,
  GAME_READY,
  GAME_PLAY,
  PAUSE,
  END
};

extern int buttons[];
extern int buttons_led[];
extern LiquidCrystal lcd;

void changeScene(Scene newScene);
void updateScene();

#endif
