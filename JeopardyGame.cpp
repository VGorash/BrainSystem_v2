#include "Game.h"

#include <arduino.h>

JeopardyGame::JeopardyGame(bool isFalstartEnabled, bool isSoundEnabled, Display& display) : Game(isFalstartEnabled, isSoundEnabled, display)
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
      playSound(TONE_TICK, DURATION_TICK);
    }
    else if (m_secondsLeft == 0)
    {
      playSound(TONE_END, DURATION_END);
      delay(DURATION_END);
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
    m_display.print("ФС");
    return;
  }
  if(m_currentPlayer > -1){
    m_display.print("К");
    m_display.print(m_currentPlayer + 1);
    return;
  }
  if (m_secondsLeft < 0)
  {
    m_display.print("--");
    return;
  }
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
