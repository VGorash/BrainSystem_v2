#include "Game.h"

#include <arduino.h>

#define JEOPARDY_GAME_TIME 7 // Game time | Время игры
#define JEOPARDY_TICKS_START 0 // Ticks start time | За сколько секунд до конца начать звуковые сигналы

JeopardyGame::JeopardyGame(bool isFalstartEnabled, Display& display, SoundMode soundMode) : Game(isFalstartEnabled, display, soundMode)
{
  m_gameTimer = new TimerMs(1000, 0, 0);
}

JeopardyGame::~JeopardyGame()
{
  delete m_gameTimer;
}

void JeopardyGame::tick()
{
  Game::tick();
  m_gameTimer->tick();
  
  if(m_gameTimer->ready())
  {
    m_secondsLeft--;
    updateDisplayState(true);
    if(m_secondsLeft <= JEOPARDY_TICKS_START && m_secondsLeft > 0)
    {
      m_sound.tick();
    }
    else if (m_secondsLeft == 0)
    {
      m_sound.end();
      delay(1000);
      cleanup();
    }
  }
}

void JeopardyGame::onStartButtonPress()
{
  if(m_isStarted || m_isPlayerButtonBlocked)
  {
    return;
  }
  m_secondsLeft = JEOPARDY_GAME_TIME;
  m_gameTimer->start();
  Game::onStartButtonPress();
  updateDisplayState(true);
}

void JeopardyGame::cleanup()
{ 
  m_gameTimer->stop();
  m_secondsLeft = -1;
  
  Game::cleanup();
}

void JeopardyGame::showTime(){
  if(m_isFalstart)
  {
    m_display.setCursor(42, 3);
    m_display.print("ФС");
    return;
  }
  if(m_currentPlayer > -1 && m_isFalstartEnabled){
    m_display.setCursor(10, 3);
    m_display.print("К");
    m_display.print(m_currentPlayer + 1);
    printPlayerTime();
    return;
  }
  if(m_currentPlayer > -1){
    m_display.setCursor(42, 3);
    m_display.print("К");
    m_display.print(m_currentPlayer + 1);
    return;
  }
  if (m_secondsLeft < 0)
  {
    m_display.setCursor(42, 3);
    m_display.print("--");
    return;
  }
  m_display.setCursor(42, 3);
  if(m_secondsLeft < 10)
  {
    m_display.print(0);
  }
  m_display.print(m_secondsLeft);
  sendUartData(UART_SLAVE_TIME_EVENT, m_secondsLeft);
}

const char* JeopardyGame::getName()
{
  return "СВОЯ ИГРА";
}
