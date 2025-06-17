#include <LiquidCrystal.h>
#include <openGLCD.h>

#include "../sceneManager.h"
#include "../cookie.h"
#include "../textUtils.h"

#include "gamePlayScene.h"
#include "gameReadyScene.h"

LcdTextLine readyText[] = {
    {0, 0, "Select Stage!!"},
    {0, 1, "Up|Down|Ok|Exit"}
};

int stageNum = 3;
stageName stageList[] = {{10, "Stage 1"}, {25, "Stage 2"}, {40, "Stage 3"}};

void drawBackground_ready() {
    GLCD.ClearScreen();

    drawGround();

    int x = 10;

    drawSquareCookie(x, cookieY);
    drawRoundCookie(116 - x, cookieY);

    for(int i = 0;i < stageNum;i++) {
        drawCenteredText(stageList[i].line, stageList[i].name);
    }
}

void showGameReadyScene() {
    GLCD.ClearScreen();
    lcd.clear();

    digitalWrite(buttons_led[0], HIGH);
    digitalWrite(buttons_led[1], HIGH);
    digitalWrite(buttons_led[2], HIGH);
    digitalWrite(buttons_led[3], HIGH);

    currentStage = 0;

    drawBackground_ready();

    setupBlinkingText(10, "Stage 1");
    setupLcdBlinkingText(readyText, 2);
}

void updateGameReadyScene() {
    if (digitalRead(buttons[0])) {
        if(currentStage > 0) {
            currentStage--;

            setupBlinkingText(stageList[currentStage].line, stageList[currentStage].name);

            delay(500);
        }
    }
    else if (digitalRead(buttons[1])) {
        if(currentStage < 2) {
            currentStage++;

            setupBlinkingText(stageList[currentStage].line, stageList[currentStage].name);

            delay(500);
        }
    }
    else if (digitalRead(buttons[2]) == LOW) {
        changeScene(GAME_PLAY);

        return;
    }
    else if (digitalRead(buttons[3]) == LOW) {
        changeScene(START);

        return;
    }

    updateBlinkingText(); // GLCD
    updateLcdBlinkingText(0); // LCD
}
