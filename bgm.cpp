#include <Arduino.h>
#include <pitches.h>

#include "bgm.h"
#include "sceneManager.h"

int speakerPin = 57;
bool isBGMPlaying = false;
int currentNoteIndex = 0;
unsigned long lastNoteTime = 0;
int noteDuration = 4;

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

void stopBGM() {
    isBGMPlaying = false;
    noTone(speakerPin);
    changeScene(PAUSE);  // but keep GAME_START = true
}

void playBGM() {
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
}