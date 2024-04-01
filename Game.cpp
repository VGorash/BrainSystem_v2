#include "Game.h"
#include "settings.h"

#include <arduino.h>

#define LED_BLINK_PERIOD 500

Game::Game(bool isFalstartEnabled) : m_isFalstartEnabled(isFalstartEnabled)
{
  m_blinkLedTimer = new TimerMs(LED_BLINK_PERIOD, 0, 0);
}

Game::~Game()
{
  cleanup();
  delete m_blinkLedTimer;
}

void Game::tick()
{
  m_blinkLedTimer->tick();
  if(m_blinkLedTimer->ready())
  {
    digitalWrite(m_blinkingLed, !digitalRead(m_blinkingLed));
  }
}

void Game::onPlayerButtonPress(const int led)
{
  if(m_isPlayerButtonBlocked)
  {
    return;
  }
  bool isFalstart = m_isFalstartEnabled && !m_isStarted;
  cleanup();
  m_isPlayerButtonBlocked = true;
  
  if(isFalstart)
  {
    blinkLed(led);
    playSound(TONE_FALSTART, DURATION_FALSTART);
    m_isFalstart = true;
  }
  else
  {
    digitalWrite(led, 1);
    playSound(TONE_PRESS, DURATION_PRESS);
  } 
}

void Game::onStopButtonPress()
{
  cleanup();
}

void Game::onStartButtonPress()
{
  if(m_isStarted || m_isPlayerButtonBlocked)
  {
    return;
  }
  m_isStarted = true;
  digitalWrite(LED_SIGNAL, 1);
  playSound(TONE_START, DURATION_START);
}

void Game::blinkLed(int led)
{
  m_blinkingLed = led;
  digitalWrite(led, 1);
  m_blinkLedTimer->start();
}

void Game::switchSound()
{
  m_isSoundEnabled = !m_isSoundEnabled;
  playSound(4000, 100);
}

void Game::playSound(int freq, int duration)
{
  if(!m_isSoundEnabled)
  {
    return;
  }
  tone(BUZZER, freq, duration);
}

void Game::cleanup()
{
  m_blinkLedTimer->stop();
  noTone(BUZZER);
  
  digitalWrite(LED_PLAYER_1, 0);
  digitalWrite(LED_PLAYER_2, 0);
  digitalWrite(LED_PLAYER_3, 0);
  digitalWrite(LED_PLAYER_4, 0);
  digitalWrite(LED_SIGNAL, 0);

  m_isStarted = false;
  m_isFalstart = false;
  m_isPlayerButtonBlocked = false;
}

Game *Game::nextGame()
{
  if(m_isFalstartEnabled)
  {
    return new JeopardyGame(false);
  }
  return new Game(true);
}

void Game::showType()
{
  digitalWrite(LED_SIGNAL, 1);
  if(m_isFalstartEnabled)
  {
    digitalWrite(LED_PLAYER_3, 1);
  }
  else
  {
    digitalWrite(LED_PLAYER_4, 1);
  }
  delay(1000);
  cleanup();
}

void Game::showGreeting()
{
  digitalWrite(LED_PLAYER_1, 1);
  playSound(1000, 300);
  delay(500);
  digitalWrite(LED_PLAYER_1, 0);
  digitalWrite(LED_PLAYER_2, 1);
  playSound(1500, 300);
  delay(500);
  digitalWrite(LED_PLAYER_2, 0);
  digitalWrite(LED_PLAYER_3, 1);
  playSound(2000, 300);
  delay(500);
  digitalWrite(LED_PLAYER_3, 0);
  digitalWrite(LED_PLAYER_4, 1);
  playSound(2500, 300);
  delay(500);
  digitalWrite(LED_PLAYER_4, 0);
}
