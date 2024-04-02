#include "Game.h"

#include <arduino.h>

#define LED_BLINK_PERIOD 500

Game::Game(bool isFalstartEnabled, Display& display) : m_isFalstartEnabled(isFalstartEnabled), m_display(display)
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

void Game::onPlayerButtonPress(int player)
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
    blinkLed(leds[player]);
    playSound(TONE_FALSTART, DURATION_FALSTART);
    m_isFalstart = true;
  }
  else
  {
    m_currentPlayer = player;
    digitalWrite(leds[player], 1);
    playSound(TONE_PRESS, DURATION_PRESS);
  } 
  updateDisplayState(true);
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
  m_currentPlayer = -1;

  updateDisplayState(true);
}

Game *Game::nextGame()
{
  if(m_isFalstartEnabled)
  {
    return new JeopardyGame(false, m_display);
  }
  return new Game(true, m_display);
}

const char* Game::getName()
{
  return "БЕЗ ОТСЧЕТА";
}

void Game::updateDisplayState(bool timeOnly)
{
  if(!timeOnly)
  {
    m_display.clear();
    m_display.setScale(1);
    m_display.home();
    m_display.print(getName());
    m_display.setCursor(110, 0);
    m_display.print(m_isFalstartEnabled ? "Ф/С" : "Б/Ф");
  }
  m_display.setScale(4);
  m_display.setCursor(42, 3);
  showTime();
  m_display.update();
}

void Game::showTime(){
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
  m_display.print("--");
}
