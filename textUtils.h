#ifndef TEXTUTILS_H
#define TEXTUTILS_H

// 외부 참조 변수
extern byte patterns[]; // 7세그먼트 숫자 패턴 배열
extern int digit_select_pin[]; // 자리 선택 핀 배열
extern int segment_pin[]; // 세그먼트 핀 배열

extern Scene currentScene; // 현재 씬 정보

// 7세그먼트 제어 함수
void clear_segments(); // 모든 세그먼트 OFF
void set_segments_for_number(int num); // 숫자에 맞게 세그먼트 설정
void show_digit(int pos, int number); // 특정 자리(pos)에 숫자 표시
void show_4_digit(int number); // 4자리 숫자 전체 표시

// GLCD 텍스트 출력 함수
void drawCenteredText(int y, const String& text, bool withBox = false); // 가운데 정렬 텍스트 출력
void drawRightAlignedText(int y, const String& text, bool withBox = false); // 오른쪽 정렬 텍스트 출력
void setupBlinkingText(int y, const String& text); // 깜빡이는 텍스트 설정
void updateBlinkingText(); // 깜빡이는 텍스트 갱신

// LCD 텍스트 구조체
struct LcdTextLine {
    uint8_t col; // 열 위치
    uint8_t row; // 행 위치
    String text; // 출력할 문자열
};

// LCD 제어
extern LiquidCrystal lcd; // LCD 객체

void setupLcdBlinkingText(const LcdTextLine* lines, int lineCount); // LCD 텍스트 깜빡임 초기 설정
void updateLcdBlinkingText(int mode); // LCD 텍스트 깜빡임 업데이트

#endif
