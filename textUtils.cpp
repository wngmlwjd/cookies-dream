#include <openGLCD.h>
#include <LiquidCrystal.h>

#include "sceneManager.h"

#include "textUtils.h"

// 7세그먼트
int SEGMENT_DELAY = 5; // 숫자 표시 사이의 시간 간격

Scene pre_scene = START;

void clear_segments() {
    // 모든 자리 선택 핀 끄기 (HIGH)
    for (int i = 0; i < 4; i++) {
        digitalWrite(digit_select_pin[i], HIGH);
    }

    // 모든 세그먼트 끄기 (LOW)
    for (int i = 0; i < 8; i++) {
        digitalWrite(segment_pin[i], LOW);
    }
}

// 해당 자리에 숫자 하나를 표시하는 함수
void show_digit(int pos, int number) { // (위치, 출력할 숫자)
    for (int i = 0; i < 4; i++) {
        if (i + 1 == pos) // 해당 자릿수의 선택 핀만 LOW로 설정
        digitalWrite(digit_select_pin[i], LOW);
        else // 나머지 자리는 HIGH로 설정
        digitalWrite(digit_select_pin[i], HIGH);
    }
    for (int i = 0; i < 8; i++) {
        boolean on_off = bitRead(patterns[number], 7 - i);
        digitalWrite(segment_pin[i], on_off);
    }
}

void show_4_digit(int _number) {
    int number = _number;
    number = number % 10000; // 4자리로 제한
    int thousands = number / 1000; // 천 자리
    number = number % 1000;
    int hundreads = number / 100; // 백 자리
    number = number % 100;
    int tens = number / 10; // 십 자리
    int ones = number % 10; // 일 자리
    if (_number > 999) show_digit(1, thousands);
    delay(SEGMENT_DELAY);
    if (_number > 99) show_digit(2, hundreads);
    delay(SEGMENT_DELAY);
    if (_number > 9) show_digit(3, tens);
    delay(SEGMENT_DELAY);
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

void drawRightAlignedText(int y, const String& text, bool withBox = false) {
    int charWidth = 6;
    int charHeight = 8;
    int padding = 3;
    int screenWidth = 128;
    int textWidth = text.length() * charWidth;
    int x = screenWidth - textWidth;  // 오른쪽 정렬

    GLCD.SelectFont(System5x7);
    GLCD.GotoXY(x, y);
    GLCD.print(text);

    if (withBox) {
        GLCD.DrawRect(x - padding, y - padding, textWidth + padding * 2 - 1, charHeight + padding * 2 - 1);
    }
}


void setupBlinkingText(int y, const String& text) {
    static int prevBlinkX = -1;
    static int prevBlinkY = -1;
    static int prevBlinkW = 0;
    static int prevBlinkH = 0;
    static String prevText = "";

    int charWidth = 6;
    int charHeight = 8;
    int padding = 3;
    int screenWidth = 128;
    int textWidth = text.length() * charWidth;
    int x = (screenWidth - textWidth) / 2;

    if(currentScene == pre_scene) {
        // 이전 깜빡임 영역 클리어 (padding 포함)
        if (prevBlinkX != -1 && prevBlinkY != -1) {
            GLCD.FillRect(prevBlinkX, prevBlinkY, prevBlinkW, prevBlinkH, WHITE); // 배경색으로 지우기
            // 이전 텍스트 원래 위치에 다시 그리기
            GLCD.GotoXY(prevBlinkX + padding, prevBlinkY + padding);
            GLCD.print(prevText);
        }
    }

    // 새 깜빡임 설정
    blinkX = x - padding;
    blinkY = y - padding;
    blinkW = textWidth + padding * 2 - 1;
    blinkH = charHeight + padding * 2 - 1;

    blinkingEnabled = true;
    blinkVisible = true;
    lastBlink = millis();

    // 새 텍스트 기본 출력
    GLCD.GotoXY(x, y);
    GLCD.print(text);

    // 이전 정보 갱신
    prevBlinkX = blinkX;
    prevBlinkY = blinkY;
    prevBlinkW = blinkW;
    prevBlinkH = blinkH;
    prevText = text;

    pre_scene = currentScene;
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

void updateLcdBlinkingText(int mode) {
    if (!lcdBlinkLines || lcdLineCount == 0) return;

    unsigned long now = millis();
    if (now - lastLcdBlink >= lcdBlinkInterval) {
        lastLcdBlink = now;
        lcdVisible = !lcdVisible;

        for (int i = 0; i < lcdLineCount; ++i) {
            int row = lcdBlinkLines[i].row;

            // 조건: mode == 2 (두 줄 모두) || mode와 row가 일치해야 함
            if (mode != 2 && row != mode) continue;

            lcd.setCursor(lcdBlinkLines[i].col, row);

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
