#include <openGLCD.h>
#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <LiquidCrystal.h>

#include "scenes/startScene.h"
#include "scenes/gameReadyScene.h"
#include "scenes/gamePlayScene.h"
#include "scenes/pauseScene.h"
#include "scenes/endScene.h"

#include "sceneManager.h"

Scene currentScene = START;

void changeScene(Scene newScene) {
  currentScene = newScene;

  // lcd.clear();
  // lcd.print(currentScene);
  // delay(500);

  // GLCD.ClearScreen();
  // lcd.clear();

  for (int i = 0; i < 4; i++) {
    digitalWrite(buttons_led[i], LOW);
  }

  switch (newScene) {
    case START:
      showStartScene();
      break;
    case GAME_READY:
      showGameReadyScene();
      break;
    case GAME_PLAY:
      showGamePlayScene();
      break;
    case PAUSE:
      showPauseScene();
      break;
    case END:
      showEndScene();
      break;
  }

  delay(500);
}

void updateScene() {
  switch (currentScene) {
    case START:
      updateStartScene();
      break;
    case GAME_READY:
      updateGameReadyScene();
      break;
    case GAME_PLAY:
      updateGamePlayScene();
      break;
    case PAUSE:
      updatePauseScene();
      break;
    case END:
      updateEndScene();
      break;
  }
}
