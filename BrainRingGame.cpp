#include "Game.h"

#include <arduino.h>

#define BRAIN_GAME_TIME 60
#define BRAIN_WA_TIME 20
#define BRAIN_TICKS_START 5
#define BRAIN_PRE_END_TIME 10


BrainRingGame::BrainRingGame(bool isFalstartEnabled, Display& display, SoundMode soundMode) : JeopardyGame(isFalstartEnabled, display, soundMode)
{
}

void BrainRingGame::tick()
{
  Game::tick();
  m_gameTimer->tick();

  if(m_gameTimer->ready())
  {
    m_secondsLeft--;
    updateDisplayState(true);
    if(m_secondsLeft == BRAIN_PRE_END_TIME - 1)
    {
      m_sound.pre_end();
    }
    if(m_secondsLeft <= BRAIN_TICKS_START && m_secondsLeft > 0)
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
    m_sound.start();
    sendUartData(UART_SLAVE_ENABLE_SIGNAL);
    m_secondsLeft = BRAIN_WA_TIME;
    updateDisplayState(true);
    m_gameTimer->start();
    return;
  }
  
  m_isStarted = true;
  digitalWrite(LED_SIGNAL, 1);
  m_sound.start();
  sendUartData(UART_SLAVE_ENABLE_SIGNAL);
  m_secondsLeft = BRAIN_GAME_TIME;
  updateDisplayState(true);
  m_gameTimer->start();
}

const char* BrainRingGame::getName()
{
  return "БРЕЙН-РИНГ";
}
