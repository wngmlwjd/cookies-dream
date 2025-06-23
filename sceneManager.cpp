#include <openGLCD.h>
#include <LiquidCrystal.h>

#include "scenes/startScene.h"
#include "scenes/gameReadyScene.h"
#include "scenes/gamePlayScene.h"
#include "scenes/pauseScene.h"
#include "scenes/endScene.h"

#include "sceneManager.h"

Scene currentScene = START;  // 현재 씬 상태

// 씬 전환 함수
void changeScene(Scene newScene) {
  currentScene = newScene;  // 현재 씬 변경

  // 버튼 LED 모두 끄기
  for (int i = 0; i < 4; i++) {
    digitalWrite(buttons_led[i], LOW);
  }

  // 새 씬에 맞는 화면 출력
  switch (newScene) {
    case START:
      showStartScene();  // 시작 화면
      break;
    case GAME_READY:
      showGameReadyScene();  // 스테이지 선택 화면
      break;
    case GAME_PLAY:
      showGamePlayScene();  // 게임 플레이 화면
      break;
    case PAUSE:
      showPauseScene();  // 일시정지 화면
      break;
    case END:
      showEndScene();  // 종료 화면
      break;
  }

  delay(500);  // 전환 후 잠시 대기
}

// 현재 씬 업데이트 함수
void updateScene() {
  switch (currentScene) {
    case START:
      updateStartScene();  // 시작 화면 업데이트
      break;
    case GAME_READY:
      updateGameReadyScene();  // 스테이지 선택 화면 업데이트
      break;
    case GAME_PLAY:
      updateGamePlayScene();  // 게임 플레이 화면 업데이트
      break;
    case PAUSE:
      updatePauseScene();  // 일시정지 화면 업데이트
      break;
    case END:
      updateEndScene();  // 종료 화면 업데이트
      break;
  }
}
