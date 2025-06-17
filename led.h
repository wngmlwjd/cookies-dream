#ifndef LED_H
#define LED_H

extern int RGB_LED[];

enum Color {
    RED,
    GREEN
};

void led_on(int color);
void led_off();

#endif