#pragma once

#include <TimerMs.h>

#include "display.h"
#include "settings.h"

// Slave mode (8 buttons)
#define UART_SLAVE_PLAYER_BUTTON_PRESSED 0x00
#define UART_SLAVE_CLEANUP 0x10
#define UART_SLAVE_ENABLE_LED 0x20
#define UART_SLAVE_BLINK_LED 0x30
#define UART_SLAVE_ENABLE_SIGNAL 0x40
#define UART_SLAVE_TIME_EVENT 0x80

class Game
{ 
  public:
    Game(bool isFalstartEnabled, Display& display);
    virtual ~Game();

    virtual void tick();
    
    virtual void onStartButtonPress();
    virtual void onStopButtonPress();
    virtual void onPlayerButtonPress(int player);
    virtual void onUartDataReceive(byte data);

    void switchSound();

    virtual Game* nextGame();

    void updateDisplayState(bool timeOnly=false);
    virtual void showTime();

  protected:
    virtual const char* getName();
    virtual void cleanup();
    void blinkLed(int led);
    void playSound(int freq, int duration);
    void sendUartData(byte command, byte payload=0x00);

  protected:
    bool m_isPlayerButtonBlocked = false;
    bool m_isStarted = false;
    bool m_isFalstart = false;

    bool m_isFalstartEnabled = false;
    bool m_isSoundEnabled = false;

    TimerMs *m_blinkLedTimer;
    int m_blinkingLed;

    int m_currentPlayer = -1;

    Display& m_display;

    int leds[4] = {LED_PLAYER_1, LED_PLAYER_2, LED_PLAYER_3, LED_PLAYER_4};
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

class EightButtonsGame : public Game
{
  public:
    EightButtonsGame(bool isFalstartEnabled, Display& display);

    void onPlayerButtonPress(int player) override;
    void onStartButtonPress() override;
    void onStopButtonPress() override;
    void onUartDataReceive(byte data) override;

    Game* nextGame() override;

  protected:
    const char* getName() override;
};
