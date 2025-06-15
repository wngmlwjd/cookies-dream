#ifndef GAMEPLAYSCENE_H
#define GAMEPLAYSCENE_H

extern LiquidCrystal lcd;
extern int buttons[4];
extern int buttons_led[4];

// cookie
extern int jumpY;
extern unsigned long jumpStartTime;
extern int jumpDelay;
extern int jumpHeight;
extern int elapsed;

void drawGround();
void drawBackground_play();
void updateJump();

void showGamePlayScene();
void updateGamePlayScene();

#endif
