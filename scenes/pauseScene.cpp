#include <openGLCD.h>
#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <LiquidCrystal.h>

#include "../textUtils.h"

#include "gamePlayScene.h"
#include "pauseScene.h"

LcdTextLine pauseText[] = {
    {0, 0, "Game Paused!!"},
};

extern bool GAME_START;
extern bool isJumping;

void showPauseScene() {
    lcd.clear();
    setupLcdBlinkingText(pauseText, 1);
    lcd.setCursor(0, 1);
    lcd.print("Resume  |   Exit");
}

void updatePauseScene() {
    updateLcdBlinkingText();

    digitalWrite(buttons_led[2], HIGH); // Resume
    digitalWrite(buttons_led[3], HIGH); // Exit

    if (digitalRead(buttons[2]) == LOW) {
        // Resume → 단순히 씬만 게임으로
        changeScene(GAME_PLAY);
    }
    else if (digitalRead(buttons[3]) == LOW) {
        // 종료 → 모든 상태 초기화 필요
        GAME_START = false;
        isJumping = false;
        noTone(speakerPin);
        changeScene(START);
    }
}
