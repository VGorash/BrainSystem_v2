#include "Game.h"
#include "settings.h"

#include <arduino.h>

JeopardyGame::JeopardyGame(bool isFalstartEnabled) : Game(isFalstartEnabled)
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
}

void JeopardyGame::cleanup()
{ 
  m_gameTimer->stop();
  
  Game::cleanup();
}

Game *JeopardyGame::nextGame()
{
  if(m_isFalstartEnabled)
  {
    return new BrainRingGame(false);
  }
  return new JeopardyGame(true);
}

void JeopardyGame::showType()
{
  digitalWrite(LED_SIGNAL, 1);
  if(m_isFalstartEnabled)
  {
    digitalWrite(LED_PLAYER_2, 1);
  }
  else
  {
    digitalWrite(LED_PLAYER_3, 1);
    digitalWrite(LED_PLAYER_4, 1);
  }
  delay(1000);
  cleanup();
}
