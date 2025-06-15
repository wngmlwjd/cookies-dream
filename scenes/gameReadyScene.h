#ifndef GAMEREADYSCENE_H
#define GAMEREADYSCENE_H

// main
extern LiquidCrystal lcd;
extern int buttons[4];
extern int buttons_led[4];

// cookie
extern int cookieY;
extern int jumpY;
extern int jumpDelay;
extern int jumpHeight;

// gamePlayScene

void showGameReadyScene();
void updateGameReadyScene();

void drawBackground_ready();

#endif
