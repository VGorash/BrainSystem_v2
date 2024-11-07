#pragma once

#include "settings.h"

#define NORMAL_TONE_START 2000
#define NORMAL_TONE_PRESS 1000
#define NORMAL_TONE_FALSTART 500
#define NORMAL_TONE_TICK 1500
#define NORMAL_TONE_END 250

#define LOUD_TONE_START 2000
#define LOUD_TONE_PRESS 1750
#define LOUD_TONE_FALSTART 1500
#define LOUD_TONE_TICK 2000
#define LOUD_TONE_END 1500

#define SOUND_DURATION_START 1000
#define SOUND_DURATION_PRESS 1000
#define SOUND_DURATION_FALSTART 500
#define SOUND_DURATION_TICK 250
#define SOUND_DURATION_END 1000

enum SoundMode{
  Off=0,
  Normal,
  Loud
};

class Sound {
public:
  Sound(SoundMode mode): m_mode(mode){};

  void start() {
    play(NORMAL_TONE_START, LOUD_TONE_START, SOUND_DURATION_START);
  };

  void press() {
    play(NORMAL_TONE_PRESS, LOUD_TONE_PRESS, SOUND_DURATION_PRESS);
  };

  void falstart() {
    play(NORMAL_TONE_FALSTART, LOUD_TONE_FALSTART, SOUND_DURATION_FALSTART);
  };

  void tick() {
    play(NORMAL_TONE_TICK, LOUD_TONE_TICK, SOUND_DURATION_TICK);
  };

  void end() {
    play(NORMAL_TONE_END, LOUD_TONE_END, SOUND_DURATION_END);
  };

  void playTone(int tone, int duration) {
    play(tone, tone, duration);
  };

  SoundMode getSoundMode()
  {
    return m_mode;
  };

private:

  void play(int normalTone, int loudTone, int duration) {
    switch(m_mode) {
      case Off: {
        return;
      }
      case Normal: {
        tone(BUZZER, normalTone, duration);
        break;
      }
      case Loud: {
        tone(BUZZER, loudTone, duration);
        break;
      }
    }
  };

private:
  SoundMode m_mode;
};