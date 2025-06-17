#ifndef TEXTUTILS_H
#define TEXTUTILS_H

extern byte patterns[];
extern int digit_select_pin[];
extern int segment_pin[];

extern Scene currentScene;

void clear_segments();
void set_segments_for_number(int num);
void show_digit(int pos, int number);
void show_4_digit(int number);

// GLCD
void drawCenteredText(int y, const String& text, bool withBox = false);
void drawRightAlignedText(int y, const String& text, bool withBox = false);
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
void updateLcdBlinkingText(int mode);

#endif
