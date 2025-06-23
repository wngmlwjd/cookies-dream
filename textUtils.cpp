#include <openGLCD.h>
#include <LiquidCrystal.h>

#include "sceneManager.h"
#include "textUtils.h"

// 7세그먼트
int SEGMENT_DELAY = 5; // 숫자 표시 사이의 시간 간격

Scene pre_scene = START; // 이전 씬 저장용

// 모든 세그먼트 OFF
void clear_segments() {
    for (int i = 0; i < 4; i++) {
        digitalWrite(digit_select_pin[i], HIGH); // 자리 선택 비활성화
    }

    for (int i = 0; i < 8; i++) {
        digitalWrite(segment_pin[i], LOW); // 세그먼트 끄기
    }
}

// 특정 자리에 숫자 하나 표시
void show_digit(int pos, int number) {
    for (int i = 0; i < 4; i++) {
        if (i + 1 == pos) digitalWrite(digit_select_pin[i], LOW); // 해당 자리만 ON
        else digitalWrite(digit_select_pin[i], HIGH); // 나머지 OFF
    }
    for (int i = 0; i < 8; i++) {
        boolean on_off = bitRead(patterns[number], 7 - i); // 비트 순서대로 추출
        digitalWrite(segment_pin[i], on_off); // 출력
    }
}

// 4자리 숫자 전체 출력
void show_4_digit(int _number) {
    int number = _number % 10000; // 4자리 제한
    int thousands = number / 1000; // 천의 자리
    number %= 1000;
    int hundreads = number / 100; // 백의 자리
    number %= 100;
    int tens = number / 10; // 십의 자리
    int ones = number % 10; // 일의 자리

    if (_number > 999) show_digit(1, thousands);
    delay(SEGMENT_DELAY);
    if (_number > 99) show_digit(2, hundreads);
    delay(SEGMENT_DELAY);
    if (_number > 9) show_digit(3, tens);
    delay(SEGMENT_DELAY);
    show_digit(4, ones);
    delay(SEGMENT_DELAY);
}

// GLCD 깜빡임 텍스트 상태 변수
static int blinkX = 0;
static int blinkY = 0;
static int blinkW = 0;
static int blinkH = 0;
static unsigned long lastBlink = 0;
static bool blinkVisible = true;
static bool blinkingEnabled = false;

// 중앙 정렬 텍스트 출력
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

// 오른쪽 정렬 텍스트 출력
void drawRightAlignedText(int y, const String& text, bool withBox) {
    int charWidth = 6;
    int charHeight = 8;
    int padding = 3;
    int screenWidth = 128;
    int textWidth = text.length() * charWidth;
    int x = screenWidth - textWidth;

    GLCD.SelectFont(System5x7);
    GLCD.GotoXY(x, y);
    GLCD.print(text);

    if (withBox) {
        GLCD.DrawRect(x - padding, y - padding, textWidth + padding * 2 - 1, charHeight + padding * 2 - 1);
    }
}

// 텍스트 깜빡임 설정
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

    if (currentScene == pre_scene) {
        if (prevBlinkX != -1 && prevBlinkY != -1) {
            GLCD.FillRect(prevBlinkX, prevBlinkY, prevBlinkW, prevBlinkH, WHITE); // 이전 깜빡임 제거
            GLCD.GotoXY(prevBlinkX + padding, prevBlinkY + padding); // 이전 텍스트 복원
            GLCD.print(prevText);
        }
    }

    blinkX = x - padding;
    blinkY = y - padding;
    blinkW = textWidth + padding * 2 - 1;
    blinkH = charHeight + padding * 2 - 1;

    blinkingEnabled = true;
    blinkVisible = true;
    lastBlink = millis();

    GLCD.GotoXY(x, y);
    GLCD.print(text);

    prevBlinkX = blinkX;
    prevBlinkY = blinkY;
    prevBlinkW = blinkW;
    prevBlinkH = blinkH;
    prevText = text;

    pre_scene = currentScene;
}

// 텍스트 깜빡임 업데이트
void updateBlinkingText() {
    if (!blinkingEnabled) return;

    unsigned long now = millis();
    if (now - lastBlink >= 400) {
        GLCD.InvertRect(blinkX, blinkY, blinkW, blinkH); // 반전 효과
        blinkVisible = !blinkVisible;
        lastBlink = now;
    }
}

// LCD 텍스트 깜빡임용 상태 변수
static const LcdTextLine* lcdBlinkLines = nullptr;
static int lcdLineCount = 0;
static bool lcdVisible = true;
static unsigned long lastLcdBlink = 0;
static const unsigned long lcdBlinkInterval = 500;

// LCD 텍스트 깜빡임 설정
void setupLcdBlinkingText(const LcdTextLine* lines, int lineCount) {
    lcdBlinkLines = lines;
    lcdLineCount = lineCount;
    lcdVisible = true;
    lastLcdBlink = millis();

    for (int i = 0; i < lineCount; ++i) {
        lcd.setCursor(lines[i].col, lines[i].row); lcd.print(lines[i].text);
    }
}

// LCD 텍스트 깜빡임 업데이트
void updateLcdBlinkingText(int mode) {
    if (!lcdBlinkLines || lcdLineCount == 0) return;

    unsigned long now = millis();
    if (now - lastLcdBlink >= lcdBlinkInterval) {
        lastLcdBlink = now;
        lcdVisible = !lcdVisible;

        for (int i = 0; i < lcdLineCount; ++i) {
            int row = lcdBlinkLines[i].row;
            if (mode != 2 && row != mode) continue;

            lcd.setCursor(lcdBlinkLines[i].col, row);

            if (lcdVisible) {
                lcd.print(lcdBlinkLines[i].text);
            } else {
                String blankStr = "";
                for (int j = 0; j < lcdBlinkLines[i].text.length(); ++j) {
                    blankStr += ' ';
                }
                lcd.print(blankStr);
            }
        }
    }
}
