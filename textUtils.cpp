#include <openGLCD.h>
#include <LiquidCrystal.h>

#include "textUtils.h"

// 7세그먼트
int SEGMENT_DELAY = 5; // 숫자 표시 사이의 시간 간격

void clear_segments() {
    for (int i = 0; i < 7; i++) {
        digitalWrite(digit_select_pin[i], HIGH);    
    }
}

void set_segments_for_number(int num) {
    byte pattern = patterns[num];

    for (int i = 0; i < 7; i++) {
        bool is_on = (pattern >> (6 - i)) & 0x01;  // 6부터 시작
        
        digitalWrite(segment_pin[i], is_on);      // 아날로그가 아니라 디지털
    }
}

void set_digit_position(int pos) {
    for (int i = 0; i < 4; i++) {
        digitalWrite(digit_select_pin[i], HIGH); // 모든 자리 끔
    }
    digitalWrite(digit_select_pin[pos - 1], LOW); // 해당 자리 켬
}

// 해당 자리에 숫자 하나를 표시하는 함수
void show_digit(int pos, int num) {
    set_digit_position(pos);
    if (num == -1) {
        clear_segments(); // 모든 세그먼트를 꺼주는 함수
    } else {
        set_segments_for_number(num);
    }
}

// 4자리 7세그먼트 표시 장치에 4자리 숫자를 표시하는 함수
void show_4_digit(int number) {
    number = number % 10000; // 4자리로 제한

    int thousands = number / 1000;
    int hundreds = (number % 1000) / 100;
    int tens = (number % 100) / 10;
    int ones = number % 10;

    bool started = false;

    if (thousands != 0) {
        show_digit(1, thousands);
        started = true;
    } else {
        show_digit(1, -1); // -1: 꺼짐
    }
    delay(SEGMENT_DELAY);

    if (hundreds != 0 || started) {
        show_digit(2, hundreds);
        started = true;
    } else {
        show_digit(2, -1);
    }
    delay(SEGMENT_DELAY);

    if (tens != 0 || started) {
        show_digit(3, tens);
        started = true;
    } else {
        show_digit(3, -1);
    }
    delay(SEGMENT_DELAY);

    // 일의 자리는 무조건 표시
    show_digit(4, ones);
    delay(SEGMENT_DELAY);
}


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

