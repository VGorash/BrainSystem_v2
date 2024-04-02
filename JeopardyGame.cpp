#include "Game.h"
#include "settings.h"

#include <arduino.h>

JeopardyGame::JeopardyGame(bool isFalstartEnabled, Display& display) : Game(isFalstartEnabled, display)
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
  updateDisplayState(true);
  m_gameTimer->start();
  Game::onStartButtonPress();
}

void JeopardyGame::cleanup()
{ 
  m_gameTimer->stop();
  m_secondsLeft = -1;
  updateDisplayState(true);
  
  Game::cleanup();
}

Game *JeopardyGame::nextGame()
{
  if(m_isFalstartEnabled)
  {
    return new BrainRingGame(false, m_display);
  }
  return new JeopardyGame(true, m_display);
}

void JeopardyGame::showTime(){
  if (m_secondsLeft < 0){
    m_display.print("--");
    return;
  }
  if(m_secondsLeft < 10){
    m_display.print(0);
  }
  m_display.print(m_secondsLeft);
}

const char* JeopardyGame::getName()
{
  return "СВОЯ ИГРА";
}
