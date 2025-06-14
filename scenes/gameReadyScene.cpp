#include <LiquidCrystal.h>
#include <openGLCD.h>

#include "../sceneManager.h"
#include "../cookie.h"
#include "../textUtils.h"

extern LiquidCrystal lcd;
extern int buttons[4];
extern int buttons_led[4];
bool GAME_START;

void showGameReadyScene() {
    GAME_START = false;

    digitalWrite(buttons_led[0], HIGH);
    digitalWrite(buttons_led[2], HIGH);
    digitalWrite(buttons_led[3], HIGH);

    GLCD.ClearScreen();
    lcd.clear();
    lcd.print("High Score:");
    lcd.setCursor(0, 1);
    lcd.print("Jump|Start|Exit");

    drawCenteredText(20, "gameReadyScene");
}

void updateGameReadyScene() {
    if (digitalRead(buttons[2]) == LOW) { // start
        lcd.setCursor(0, 1);
        lcd.print("Jump   |   Pause");

        GAME_START = true;

        changeScene(GAME_PLAY);
    }
    else if (digitalRead(buttons[3]) == LOW) { // exit
        changeScene(START);
    }
}
