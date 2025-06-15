#include <openGLCD.h>
#include <LiquidCrystal.h>

#include "../textUtils.h"

#include "startScene.h"

LcdTextLine startText[] = {
    {0, 0, "Press Button"},
    {6, 1, "to Start!!"}
};

void showStartScene() {
    GLCD.ClearScreen();
    lcd.clear();

    drawCenteredText(20, "Square's Dream");
    drawCenteredText(40, "start");

    setupBlinkingText(40, "start");
    setupLcdBlinkingText(startText, 2);
}

void updateStartScene() {
    int button = 0;

    // 버튼 LED
    digitalWrite(buttons_led[button], HIGH);
    updateBlinkingText(); // GLCD
    updateLcdBlinkingText(); // LCD

    if (digitalRead(buttons[button])) {
        changeScene(GAME_READY);
    }
}