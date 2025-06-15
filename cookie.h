#ifndef COOKIE_H
#define COOKIE_H

extern const int cookieX;
extern const int cookieY;

extern unsigned long totalPauseDuration;

extern bool RESUME;
extern unsigned long jumpStartTime_play;

// 네모난 쿠키 그리기 함수
void drawSquareCookie(int x, int y);
void drawSmileCookie(int x, int y);

// 동그란 쿠키 그리기 함수
void drawRoundCookie(int x, int y);

bool updateJump(bool isJumping, unsigned long jumpStartTime);

#endif
