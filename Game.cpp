#include "Game.h"

#include <arduino.h>

#define LED_BLINK_PERIOD 500

Game::Game(bool isFalstartEnabled, Display& display, SoundMode soundMode) : m_isFalstartEnabled(isFalstartEnabled), m_display(display), m_sound(soundMode) 
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
    // local button
    if(player < 4)
    {
      blinkLed(leds[player]);
    }
    sendUartData(UART_SLAVE_BLINK_LED, player);
    m_sound.falstart();
    m_isFalstart = true;
    updateDisplayState(true);
    delay(500);
  }
  else
  {
    // local button
    if(player < 4)
    {
      digitalWrite(leds[player], 1);
    }
    sendUartData(UART_SLAVE_ENABLE_LED, player);
    m_currentPlayer = player;
    m_sound.press();
    updateDisplayState(true);
    delay(1000);
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
  sendUartData(UART_SLAVE_ENABLE_SIGNAL);
  m_sound.start();
  m_startTime = millis();
}

void Game::blinkLed(int led)
{
  m_blinkingLed = led;
  digitalWrite(led, 1);
  m_blinkLedTimer->start();
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
  sendUartData(UART_SLAVE_CLEANUP);
}

static Game* Game::fromState(const State &state, Display& display)
{
  Game* game;
  switch(state.gameNumber)
  {
    case(1):
    {
      game = new JeopardyGame(state.isFalstartEnabled, display, state.soundMode);
      break;
    }
    case(2):
    {
      game = new BrainRingGame(state.isFalstartEnabled, display, state.soundMode);
      break;
    }
    case(3):
    {
      game = new EightButtonsGame(state.isFalstartEnabled, display, state.soundMode);
      break;
    }
    default:
    {
      game = new Game(state.isFalstartEnabled, display, state.soundMode);
    }
  }
  game->updateDisplayState();
  return game;
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
    if (m_sound.getSoundMode() == SoundMode::Off) {
      m_display.setCursor(38, 7);
      m_display.print("без звука");
    }
    if (m_sound.getSoundMode() == SoundMode::Loud) {
      m_display.setCursor(38, 7);
      m_display.print("  громко");
    }
  }
  m_display.setScale(4);
  m_display.setCursor(0, 3);
  m_display.print("        ");
  showTime();
  m_display.update();
}

void Game::showTime(){
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
  m_display.setCursor(42, 3);
  m_display.print("--");
}

void Game::printPlayerTime()
{
  if (!m_isFalstartEnabled) {
    return;
  }
  m_display.setScale(2);
  m_display.print(" ");

  int t = millis() - m_startTime - 120;
  t = t > 0 ? t : 0;

  char c[6];
  sprintf(c, "%d", t / 1000);
  m_display.print(c);
  m_display.print(",");

  if (t > 1000){
    sprintf(c, "%d", (t % 1000) / 100);
  }
  else{
    sprintf(c, "%03d", t % 1000);
  }  
  m_display.print(c);
  
  m_display.setScale(4);
}

void Game::sendUartData(byte command, byte payload)
{
  Serial.write(command | payload);
}

void Game::onUartDataReceive(byte data)
{
  byte command = data & 0xF0;
  byte payload = data & 0x0F;

  if(command == UART_SLAVE_PLAYER_BUTTON_PRESSED)
  {
    if(payload < 4){
      onPlayerButtonPress(payload + 4);
    }
  }
}
