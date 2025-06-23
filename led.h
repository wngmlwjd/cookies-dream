#ifndef LED_H
#define LED_H

extern int RGB_LED[]; // RGB LED 핀 배열

enum Color {
  RED, // 빨간색
  GREEN // 초록색
};

void led_on(int color); // LED 켜기 함수
void led_off(); // LED 끄기 함수

#endif
