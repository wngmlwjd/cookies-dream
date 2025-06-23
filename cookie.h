#ifndef COOKIE_H                      // 헤더 파일 중복 포함 방지 시작
#define COOKIE_H                      // COOKIE_H 매크로 정의

extern const int cookieX;             // 쿠키의 X 좌표 외부 선언
extern const int cookieY;             // 쿠키의 Y 좌표 외부 선언

extern unsigned long totalPauseDuration;   // 총 일시정지 시간 외부 선언

extern unsigned long jumpStartTime_play;   // 점프 시작 시간 외부 선언

extern bool RESUME;                   // 재개 상태 변수 외부 선언
extern bool CHECK;                    // 체크 상태 변수 외부 선언

// 네모난 쿠키를 그리는 함수 선언
void drawSquareCookie(int x, int y);
// 웃는 표정 네모 쿠키 그리기 함수 선언
void drawSmileCookie(int x, int y);

// 동그란 쿠키를 그리는 함수 선언
void drawRoundCookie(int x, int y);

// 점프 상태를 업데이트하는 함수 선언
// isJumping: 점프 중인지 여부, jumpStartTime: 점프 시작 시간
bool updateJump(bool isJumping, unsigned long jumpStartTime);

#endif                              // 헤더 파일 중복 포함 방지 끝
