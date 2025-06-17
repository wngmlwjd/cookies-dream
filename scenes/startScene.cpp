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

    digitalWrite(buttons_led[0], HIGH);

    drawCenteredText(20, "Square's Dream");
    drawCenteredText(40, "start");

    setupBlinkingText(40, "start");
    setupLcdBlinkingText(startText, 2);
}

void updateStartScene() {
    updateBlinkingText(); // GLCD
    updateLcdBlinkingText(2); // LCD

    if (digitalRead(buttons[0])) {
        changeScene(GAME_READY);
    }
}