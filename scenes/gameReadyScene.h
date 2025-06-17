#ifndef GAMEREADYSCENE_H
#define GAMEREADYSCENE_H

struct stageName {
    int line;
    String name;
};

// main
extern LiquidCrystal lcd;
extern int buttons[4];
extern int buttons_led[4];

// cookie
extern int jumpY;
extern int jumpDelay;
extern int jumpHeight;

extern int currentStage;

// gamePlayScene

void showGameReadyScene();
void updateGameReadyScene();

void drawBackground_ready();

#endif
