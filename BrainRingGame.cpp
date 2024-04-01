#include "Game.h"
#include "settings.h"

#include <arduino.h>


BrainRingGame::BrainRingGame(bool isFalstartEnabled) : JeopardyGame(isFalstartEnabled){}

void BrainRingGame::tick()
{
  Game::tick();
  m_gameTimer->tick();

  if(m_gameTimer->ready())
  {
    m_secondsLeft--;
    if(m_secondsLeft == BRAIN_PRE_END_TIME - 1)
    {
      playSound(TONE_TICK, DURATION_END);
    }
    if(m_secondsLeft <= BRAIN_TICKS_START && m_secondsLeft > 0)
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


void BrainRingGame::onStartButtonPress()
{
  if(m_isStarted)
  {
    return;
  }
  if(m_isFalstart)
  {
    m_isFalstart = false;
    return;
  }
  if(m_isPlayerButtonBlocked) // start wrong answer mode
  {
    cleanup();
    m_isStarted = true;
    digitalWrite(LED_SIGNAL, 1);
    playSound(TONE_START, DURATION_START);
    m_secondsLeft = BRAIN_WA_TIME;
    m_gameTimer->start();
    return;
  }
  
  m_isStarted = true;
  digitalWrite(LED_SIGNAL, 1);
  playSound(TONE_START, DURATION_START);
  m_secondsLeft = BRAIN_GAME_TIME;
  m_gameTimer->start();
}

Game *BrainRingGame::nextGame()
{
  if(m_isFalstartEnabled)
  {
    return new Game(false);
  }
  return new BrainRingGame(true);
}

void BrainRingGame::showType()
{
  digitalWrite(LED_SIGNAL, 1);
  if(m_isFalstartEnabled)
  {
    digitalWrite(LED_PLAYER_2, 1);
    digitalWrite(LED_PLAYER_3, 1);
  }
  else
  {
    digitalWrite(LED_PLAYER_2, 1);
    digitalWrite(LED_PLAYER_4, 1);
  }
  delay(1000);
  cleanup();
}
