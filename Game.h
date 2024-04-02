#pragma once

#include <TimerMs.h>

#include "display.h"

class Game
{ 
  public:
    Game(bool isFalstartEnabled, Display& display);
    virtual ~Game();

    virtual void tick();
    
    virtual void onStartButtonPress();
    virtual void onStopButtonPress();
    virtual void onPlayerButtonPress(const int led);

    void switchSound();

    virtual Game* nextGame();

    void updateDisplayState(bool timeOnly=false);
    virtual void showTime();

  protected:
    virtual const char* getName();
    virtual void cleanup();
    void blinkLed(int led);
    void playSound(int freq, int duration);

  protected:
    bool m_isPlayerButtonBlocked = false;
    bool m_isStarted = false;
    bool m_isFalstart = false;

    bool m_isFalstartEnabled = false;
    bool m_isSoundEnabled = true;

    TimerMs *m_blinkLedTimer;
    int m_blinkingLed;

    Display& m_display;
};

class JeopardyGame : public Game
{
  public:
    JeopardyGame(bool isFalstartEnabled, Display& display);
    ~JeopardyGame() override;

    void tick() override;

    void onStartButtonPress() override;

    Game* nextGame() override;

   protected:
    void cleanup() override;
    const char* getName() override;
    
    void showTime() override;

   protected:
    TimerMs *m_gameTimer;
    int m_secondsLeft=-1;
    
};

class BrainRingGame : public JeopardyGame
{
  public:
    BrainRingGame(bool isFalstartEnabled, Display& display);
  
    void tick() override;
  
    void onStartButtonPress() override;

    Game* nextGame() override;

  protected:
    const char* getName() override;

};
