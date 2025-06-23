#include <Arduino.h>

#include "led.h"

int RGB_CODE[2][3] = {{0, 255, 255}, {255, 0, 255}}; // RGB 색상 코드 배열

void led_on(int color) {
    for(int i = 0; i < 3; i++) analogWrite(RGB_LED[i], RGB_CODE[color][i]); // 해당 색상으로 LED 켜기
}

void led_off() {
    for(int i = 0; i < 3; i++) analogWrite(RGB_LED[i], 255); // LED 끄기 (최대 밝기)
}
