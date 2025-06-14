#include <pitches.h>
#include <LiquidCrystal.h>
#include <openGLCD.h>

#include "../sceneManager.h"
#include "../cookie.h"
#include "../textUtils.h"

extern LiquidCrystal lcd;
extern int buttons[4];
extern int buttons_led[4];
extern bool GAME_START;

const int speakerPin = 57;
bool isBGMPlaying = false;
int currentNoteIndex = 0;
unsigned long lastNoteTime = 0;
int noteDuration = 4;

// 점프 상태 변수
bool isJumping = false;
unsigned long jumpStartTime = 0;
const int jumpHeight = 15;
const int jumpStep = 1;
const int jumpDelay = 30;
int jumpY = 0;

// 버튼 상태 추적
bool prevJumpButtonState = LOW;

int melody[] = {
    NOTE_C4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4,
    NOTE_D4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_E4,
    NOTE_F4, NOTE_E4, NOTE_F4, 0,
    NOTE_D4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_F4,
    NOTE_E4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_G4,
    NOTE_F4, NOTE_G4, 0,
    NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_C5, NOTE_C5,
    NOTE_G4, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_G4, NOTE_G4, NOTE_A4,
    NOTE_B4, NOTE_C5, 0, 0,
    NOTE_F4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_D4,
    NOTE_C4, NOTE_D4, 0, 0, 0,
    NOTE_C4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4,
    NOTE_D4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_F4,
    NOTE_E4, NOTE_F4, 0,
    NOTE_D4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_E4,
    NOTE_F4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_G4,
    NOTE_F4, NOTE_G4, NOTE_F4, NOTE_G4, 0,
    NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_C5,
    NOTE_C5, NOTE_G4, NOTE_C5, NOTE_C5, NOTE_C5,
    NOTE_G4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, 0, 0,
    NOTE_F4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_E4,
    NOTE_D4, NOTE_B3, NOTE_C4, 0
};

void drawGround() {
    GLCD.FillRect(0, 55, 128, 10);
}

void drawBackground() {
    GLCD.ClearScreen();
    drawGround();
}

void updateJump() {
    if (!isJumping) return;

    unsigned long now = millis();
    int elapsed = now - jumpStartTime;
    int totalFrames = (jumpHeight * 2);
    int currentFrame = elapsed / jumpDelay;

    if (currentFrame >= totalFrames) {
        isJumping = false;
        jumpY = 0;
        return;
    }

    if (currentFrame < jumpHeight) {
        jumpY = currentFrame;
    } else {
        jumpY = totalFrames - currentFrame;
    }
}

void showGamePlayScene() {
    digitalWrite(buttons_led[0], HIGH);
    digitalWrite(buttons_led[3], HIGH);

    drawBackground();

    lcd.clear();
    lcd.print("High Score:");
    lcd.setCursor(0, 1);
    lcd.print("Jump   |   Pause");

    drawCenteredText(20, "gameScene");
}

void updateGamePlayScene() {
    // Pause (buttons[3]) - LOW일 때 눌림 (게임 중)
    if (digitalRead(buttons[3]) == LOW && GAME_START) {
        isBGMPlaying = false;
        noTone(speakerPin);
        changeScene(PAUSE);  // but keep GAME_START = true
        return;
    }

    // 점프 버튼 처리
    bool currentJumpButtonState = digitalRead(buttons[0]);  // HIGH일 때 눌림

    updateJump();
    drawBackground();
    drawSquareCookie(cookieX, cookieY - jumpY);

    // 배경음악 계속 재생
    unsigned long currentTime = millis();
    int noteLength = 1000 / noteDuration;

    if (currentTime - lastNoteTime >= noteLength) {
        int note = melody[currentNoteIndex];
        if (note == 0) {
            noTone(speakerPin);
        } else {
            tone(speakerPin, note, noteLength);
        }

        lastNoteTime = currentTime;
        currentNoteIndex++;

        if (currentNoteIndex >= sizeof(melody) / sizeof(int)) {
            currentNoteIndex = 0;
        }
    }

    // Jump 트리거
    if (prevJumpButtonState == LOW && currentJumpButtonState == HIGH && !isJumping) {
        isJumping = true;
        jumpStartTime = millis();
    }
    prevJumpButtonState = currentJumpButtonState;
}
