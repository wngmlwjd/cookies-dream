#ifndef GAMEPLAYSCENE_H
#define GAMEPLAYSCENE_H

#include "gameReadyScene.h"

extern LiquidCrystal lcd;
extern int buttons[4];
extern int buttons_led[4];

// cookie
extern int jumpY;
extern unsigned long jumpStartTime;
extern int jumpDelay;
extern int jumpHeight;
extern int elapsed;

extern int currentNoteIndex;
extern unsigned long lastNoteTime;
extern bool isBGMPlaying;
extern int currentStage;

extern bool finalCookieSpawned;
extern unsigned long lastObstacleSpawnTime;

extern int pre_elapsed;
extern int now_elapsed;

extern int gameScore;

extern stageName stageList[];

void drawGround();
void drawBackground_play();
void updateJump();

void showGamePlayScene();
void updateGamePlayScene();

void updateFinalCookieMeeting();
bool isFinalMeetingStarted();
void startFinalMeeting();


#endif
