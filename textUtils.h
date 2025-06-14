#ifndef TEXTUTILS_H
#define TEXTUTILS_H

void drawCenteredText(int y, const String& text, bool withBox = false);
void setupBlinkingText(int y, const String& text);
void updateBlinkingText();

// LCD 텍스트 깜빡이기용 구조체
struct LcdTextLine {
    uint8_t col;
    uint8_t row;
    String text;
};

// LCD
extern LiquidCrystal lcd;

void setupLcdBlinkingText(const LcdTextLine* lines, int lineCount);
void updateLcdBlinkingText();

#endif
