#include <openGLCD.h>
#include <LiquidCrystal.h>

#include "textUtils.h"

// GLCD
static int blinkX = 0;
static int blinkY = 0;
static int blinkW = 0;
static int blinkH = 0;
static unsigned long lastBlink = 0;
static bool blinkVisible = true;
static bool blinkingEnabled = false;

void drawCenteredText(int y, const String& text, bool withBox) {
    int charWidth = 6;
    int charHeight = 8;
    int padding = 3;
    int screenWidth = 128;
    int textWidth = text.length() * charWidth;
    int x = (screenWidth - textWidth) / 2;

    GLCD.SelectFont(System5x7);
    GLCD.GotoXY(x, y);
    GLCD.print(text);

    if (withBox) {
        GLCD.DrawRect(x - padding, y - padding, textWidth + padding * 2 - 1, charHeight + padding * 2 - 1);
    }
}

void setupBlinkingText(int y, const String& text) {
    int charWidth = 6;
    int charHeight = 8;
    int padding = 3;
    int screenWidth = 128;
    int textWidth = text.length() * charWidth;
    int x = (screenWidth - textWidth) / 2;

    blinkX = x - padding;
    blinkY = y - padding;
    blinkW = textWidth + padding * 2 - 1;
    blinkH = charHeight + padding * 2 - 1;
    blinkingEnabled = true;
    blinkVisible = true;
    lastBlink = millis();
}

void updateBlinkingText() {
    if (!blinkingEnabled) return;

    unsigned long now = millis();
    if (now - lastBlink >= 400) {
        GLCD.InvertRect(blinkX, blinkY, blinkW, blinkH);
        blinkVisible = !blinkVisible;
        lastBlink = now;
    }
}

static const LcdTextLine* lcdBlinkLines = nullptr;
static int lcdLineCount = 0;
static bool lcdVisible = true;
static unsigned long lastLcdBlink = 0;
static const unsigned long lcdBlinkInterval = 500;

void setupLcdBlinkingText(const LcdTextLine* lines, int lineCount) {
    lcdBlinkLines = lines;
    lcdLineCount = lineCount;
    lcdVisible = true;
    lastLcdBlink = millis();

    for (int i = 0; i < lineCount; ++i) {
        lcd.setCursor(lines[i].col, lines[i].row);
        lcd.print(lines[i].text);
    }
}

void updateLcdBlinkingText() {
    if (!lcdBlinkLines || lcdLineCount == 0) return;

    unsigned long now = millis();
    if (now - lastLcdBlink >= lcdBlinkInterval) {
        lastLcdBlink = now;
        lcdVisible = !lcdVisible;

        for (int i = 0; i < lcdLineCount; ++i) {
            lcd.setCursor(lcdBlinkLines[i].col, lcdBlinkLines[i].row);

            if (lcdVisible) {
                lcd.print(lcdBlinkLines[i].text);
            }
            else {
                String blankStr = "";

                for (int j = 0; j < lcdBlinkLines[i].text.length(); ++j) {
                    blankStr += ' ';
                }

                lcd.print(blankStr);
            }
        }
    }
}

