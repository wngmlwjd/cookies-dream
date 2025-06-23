#include <Arduino.h>               // 아두이노 기본 라이브러리 포함
#include <pitches.h>               // 음계 정의 포함

#include "bgm.h"                   // 배경음악 함수 선언 포함
#include "sceneManager.h"          // 씬 관리 함수 포함

int speakerPin = 57;                // 스피커 연결 핀 번호
bool isBGMPlaying = false;          // 배경음악 재생 상태 변수
int currentNoteIndex = 0;           // 현재 재생 중인 음표 인덱스
unsigned long lastNoteTime = 0;     // 마지막 음표 재생 시각 (밀리초)
int noteDuration = 4;               // 음표 길이 (분수 형태, 4분음표 등)

// 배경음악 멜로디 배열 (음표 시퀀스)
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

// 배경음악 정지 함수
void stopBGM() {
    isBGMPlaying = false;            // 재생 상태 false로 설정
    noTone(speakerPin);              // 스피커에서 음 제거
    changeScene(PAUSE);              // 씬을 일시정지 씬으로 변경 (게임은 계속)
}

// 배경음악 재생 함수 (멜로디 반복 재생)
void playBGM() {
    unsigned long currentTime = millis();         // 현재 시간 (밀리초)
    int noteLength = 1000 / noteDuration;         // 음표 길이 계산 (밀리초 단위)

    // Serial.println("BGM");                      // 디버그 출력 (필요 시 주석 해제)

    // 마지막 음표 재생 후 일정 시간이 지나면 다음 음표 재생
    if (currentTime - lastNoteTime >= noteLength) {
        int note = melody[currentNoteIndex];      // 현재 음표

        if (note == 0) {                          // 휴지부 음표일 경우
            noTone(speakerPin);                   // 스피커 소리 끔
        } else {
            tone(speakerPin, note, noteLength);  // 음표 재생
        }

        lastNoteTime = currentTime;               // 마지막 음표 재생 시각 갱신
        currentNoteIndex++;                        // 다음 음표 인덱스 증가

        // 멜로디 끝에 도달하면 처음으로 되돌림
        if (currentNoteIndex >= sizeof(melody) / sizeof(int)) {
            currentNoteIndex = 0;
        }
    }
}
