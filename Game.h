#pragma once

#include <TimerMs.h>

class Game
{ 
  public:
    Game(bool isFalstartEnabled);
    virtual ~Game();

    virtual void tick();
    
    virtual void onStartButtonPress();
    virtual void onStopButtonPress();
    virtual void onPlayerButtonPress(const int led);

    void switchSound();

    virtual Game* nextGame();
    // shows game number in binary system using players leds
    virtual void showType();
    void showGreeting();

  protected:
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
};

class JeopardyGame : public Game
{
  public:
    JeopardyGame(bool isFalstartEnabled);
    ~JeopardyGame() override;

    void tick() override;

    void onStartButtonPress() override;

    Game* nextGame() override;
    void showType() override;

   protected:
    void cleanup() override;

   protected:
    TimerMs *m_gameTimer;
    int m_secondsLeft;
    
};

class BrainRingGame : public JeopardyGame
{
  public:
    BrainRingGame(bool isFalstartEnabled);
  
    void tick() override;
  
    void onStartButtonPress() override;

    Game* nextGame() override;
    void showType() override;

};
