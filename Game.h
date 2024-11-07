#pragma once

#include <TimerMs.h>

#include "display.h"
#include "settings.h"
#include "sound.h"
#include "Storage.h"
#include "Runnable.h"

// Slave mode (8 buttons)
#define UART_SLAVE_PLAYER_BUTTON_PRESSED 0x00
#define UART_SLAVE_CLEANUP 0x10
#define UART_SLAVE_ENABLE_LED 0x20
#define UART_SLAVE_BLINK_LED 0x30
#define UART_SLAVE_ENABLE_SIGNAL 0x40
#define UART_SLAVE_TIME_EVENT 0x80

class Game : public Runnable
{ 
  public:
    Game(bool isFalstartEnabled, Display& display, SoundMode soundMode);
    virtual ~Game();

    void tick() override;
    
    void onStartButtonPress() override;
    void onStopButtonPress() override;
    void onPlayerButtonPress(int player) override;
    void onUartDataReceive(byte data) override;

    static Game* fromState(const State& state, Display& display);

    const char* getName();

  
  public:
    const static int totalGames = 4;

  protected:
    virtual void cleanup();
    virtual void showTime();
    void updateDisplayState(bool timeOnly=false);
    void blinkLed(int led);
    void sendUartData(byte command, byte payload=0x00);
    void printPlayerTime();

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
    Sound m_sound;

    unsigned long m_startTime;

    const int leds[4] = {LED_PLAYER_1, LED_PLAYER_2, LED_PLAYER_3, LED_PLAYER_4};
};

class JeopardyGame : public Game
{
  public:
    JeopardyGame(bool isFalstartEnabled, Display& display, SoundMode soundMode);
    ~JeopardyGame() override;

    void tick() override;

    void onStartButtonPress() override;

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
    BrainRingGame(bool isFalstartEnabled, Display& display, SoundMode soundMode);
  
    void tick() override;
  
    void onStartButtonPress() override;

  protected:
    const char* getName() override;

};

class EightButtonsGame : public Game
{
  public:
    EightButtonsGame(bool isFalstartEnabled, Display& display, SoundMode soundMode);

    void onPlayerButtonPress(int player) override;
    void onStartButtonPress() override;
    void onStopButtonPress() override;
    void onUartDataReceive(byte data) override;

  protected:
    const char* getName() override;
};
