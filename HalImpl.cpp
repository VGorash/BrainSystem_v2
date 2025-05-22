#include <Arduino.h>
#include <EEPROM.h>

#include "HalImpl.h"

#define LED_PLAYER_4 6
#define LED_PLAYER_3 5
#define LED_PLAYER_2 8
#define LED_PLAYER_1 7
#define LED_SIGNAL 13

#define BUTTON_PLAYER_4 A3
#define BUTTON_PLAYER_3 A2
#define BUTTON_PLAYER_2 A1
#define BUTTON_PLAYER_1 A0
#define BUTTON_START 2
#define BUTTON_FUNC 4
#define BUTTON_STOP 3

#define BUZZER 9 

#define SOUND_TONE_START 2000
#define SOUND_TONE_PRESS 1000
#define SOUND_TONE_FALSTART 500
#define SOUND_TONE_TICK 1500
#define SOUND_TONE_SIGNAL 1500
#define SOUND_TONE_END 250

#define SOUND_DURATION_START 1000
#define SOUND_DURATION_PRESS 1000
#define SOUND_DURATION_FALSTART 500
#define SOUND_DURATION_TICK 250
#define SOUND_DURATION_SIGNAL 1000
#define SOUND_DURATION_END 1000

using namespace vgs;

constexpr int playerButtonPins[NUM_PLAYERS] = {BUTTON_PLAYER_1, BUTTON_PLAYER_2, BUTTON_PLAYER_3, BUTTON_PLAYER_4};
constexpr int playerLedPins[NUM_PLAYERS] = {LED_PLAYER_1, LED_PLAYER_2, LED_PLAYER_3, LED_PLAYER_4};

HalImpl::HalImpl()
{
  m_blinkTimer.setTime(500);
  m_blinkTimer.setPeriodMode(true);
}

void HalImpl::init()
{
  Serial.begin(9600);
  Wire.setClock(800000L);
  
  m_display.init();
  m_display.textMode(BUF_REPLACE);

  for(int i=0; i<NUM_PLAYERS; i++)
  {
    m_playerButtons[i].init(playerButtonPins[i], INPUT);
    m_playerButtons[i].setDebTimeout(10);
    pinMode(playerLedPins[i], OUTPUT);
  }

  m_startButton.init(BUTTON_START, INPUT);
  m_stopButton.init(BUTTON_STOP, INPUT);
  m_funcButton.init(BUTTON_FUNC, INPUT);

  pinMode(LED_SIGNAL, OUTPUT);
}

void HalImpl::tick()
{
  if(m_blinkTimer.tick(*this))
  {
    m_blinkState = !m_blinkState;

    for(int i=0; i<NUM_PLAYERS; i++)
    {
      if(m_blinkingLeds[i])
      {
        digitalWrite(playerLedPins[i], m_blinkState);
      }
    }
  }

  m_startButton.tick();
  m_stopButton.tick();
  m_funcButton.tick();

  for(int i=0; i<NUM_PLAYERS; i++)
  {
    m_playerButtons[i].tick();
  }

  m_link.tick();
}

ButtonState HalImpl::getButtonState()
{
  ButtonState s;

  for(int i=0; i<NUM_PLAYERS; i++)
  {
    if(m_playerButtons[i].press())
    {
      s.player = i;
      break;
    }
  }

  if(s.player < 0 && m_link.getCommand() == link::Command::PlayerButton)
  {
    s.player = m_link.getData() + NUM_PLAYERS;
  }

  if(m_startButton.press())
  {
    s.start = true;
  }
  if(m_stopButton.press())
  {
    s.stop = true;
  }
  if(m_funcButton.click())
  {
    s.enter = true;
  }
  if(m_funcButton.hold())
  {
    s.menu = true;
  }

  return s;
}

void HalImpl::playerLedOn(int player)
{
  playerLedOn(player, true);
}

void HalImpl::playerLedOn(int player, bool useLink)
{
  if (player < 0)
  {
    return;
  }

  if(player < NUM_PLAYERS)
  {
    digitalWrite(playerLedPins[player], 1);
    sendLinkCommand(useLink, link::Command::DisplayPlayerLedOn, player);
    return;
  }

  player -= NUM_PLAYERS;

  if(player < UART_LINK_MAX_PLAYERS)
  {
    sendLinkCommand(useLink, link::Command::PlayerLedOn, player);
  }
}

void HalImpl::playerLedBlink(int player)
{
  playerLedBlink(player, true);
}

void HalImpl::playerLedBlink(int player, bool useLink)
{
  if (player < 0)
  {
    return;
  }

  if(player < NUM_PLAYERS)
  {
    digitalWrite(playerLedPins[player], 1);
    m_blinkingLeds[player] = true;

    sendLinkCommand(useLink, link::Command::DisplayPlayerLedBlink, player);

    if(!m_blinkTimer.isStarted())
    {
      m_blinkState = 1;
      m_blinkTimer.start(*this);
    }

    return;
  }

  player -= NUM_PLAYERS;

  if(player < UART_LINK_MAX_PLAYERS)
  {
    sendLinkCommand(useLink, link::Command::PlayerLedBlink, player);
  }
}

void HalImpl::signalLedOn()
{
  signalLedOn(true);
}

void HalImpl::signalLedOn(bool useLink)
{
  if(m_signalLightEnabled)
  {
    digitalWrite(LED_SIGNAL, 1);
  }

  sendLinkCommand(useLink, link::Command::SignalLedOn);
}

void HalImpl::ledsOff()
{
  ledsOff(true);
}

void HalImpl::ledsOff(bool useLink)
{
  digitalWrite(LED_SIGNAL, 0);

  for(int i=0; i<NUM_PLAYERS; i++)
  {
    digitalWrite(playerLedPins[i], 0);
    m_blinkingLeds[i] = false;
  }

  m_blinkTimer.stop();

  sendLinkCommand(useLink, link::Command::LedsOff);
}

void HalImpl::setSignalLightEnabled(bool enabled)
{
  m_signalLightEnabled = enabled;
}

void HalImpl::sound(HalSound soundType)
{
  if(m_soundMode == HalSoundMode::Disabled)
  {
    noTone(BUZZER);
    return;
  }

  switch(soundType)
  {
    case HalSound::Start:
      sound(SOUND_TONE_START, SOUND_DURATION_START);
      break;
    case HalSound::Press:
      sound(SOUND_TONE_PRESS, SOUND_DURATION_PRESS);
      break;
    case HalSound::Falstart:
      sound(SOUND_TONE_FALSTART, SOUND_DURATION_FALSTART);
      break;
    case HalSound::Tick:
      sound(SOUND_TONE_TICK, SOUND_DURATION_TICK);
      break;
    case HalSound::Signal:
      sound(SOUND_TONE_SIGNAL, SOUND_DURATION_SIGNAL);
      break;
    case HalSound::End:
      sound(SOUND_TONE_END, SOUND_DURATION_END);
      break;
    case HalSound::None:
      noTone(BUZZER);
      break;
  }
}

void HalImpl::sound(unsigned int frequency, unsigned int duration)
{
  tone(BUZZER, frequency, duration);
}

void HalImpl::setSoundMode(HalSoundMode mode)
{
  m_soundMode = mode;
}

bool pressTimeEnabled(const GameDisplayInfo& info)
{
  return info.pressTime >= 0 && info.mode == GameMode::Falstart;
}

void HalImpl::showTime(const GameDisplayInfo& info)
{
  switch(info.state)
  {
    case GameState::Press:
    {
      m_display.setCursor(pressTimeEnabled(info) ? 10 : 42, 3);
      m_display.print("К");
      m_display.print(info.player + 1);
      showPressTime(info);
    }
    break;

    case GameState::Falstart:
    {
      m_display.setCursor(42, 3);
      m_display.print("ФС");
    }
    break;

    case GameState::Idle:
    {
      m_display.setCursor(42, 3);
      m_display.print("--");
    }
    break;

    case GameState::Countdown:
    {
      m_display.setCursor(42, 3);

      if(info.gameTime >= 0)
      {
        sendLinkCommand(true, link::Command::UpdateTime, info.gameTime);

        if(info.gameTime < 10)
        {
          m_display.print(0);
        }

        m_display.print(info.gameTime);
      }
      else
      {
        m_display.print("--");
      }
    }
    break;
  }
}


void HalImpl::showPressTime(const GameDisplayInfo& info)
{
  if (!pressTimeEnabled(info))
  {
    return;
  }

  m_display.setScale(2);
  m_display.print(" ");

  int t = info.pressTime;

  char c[6];
  sprintf(c, "%d", t / 1000);
  m_display.print(c);
  m_display.print(",");

  if (t > 1000){
    sprintf(c, "%d", (t % 1000) / 100);
  }
  else{
    sprintf(c, "%02d", (t % 1000) / 10);
  }  

  m_display.print(c);
  m_display.print("c");
}


void HalImpl::updateDisplay(const GameDisplayInfo& info)
{
  bool questionsMode = String(info.name) == String("ЧГК");
  m_displayForceUpdate = m_displayForceUpdate || (m_state.display_mode != DisplayMode::Game) || (questionsMode && info.customInt > 0 && info.state == GameState::Idle);
  m_state.display_mode = DisplayMode::Game;

  if(m_displayForceUpdate)
  {
    constexpr const char* modeNames[2] = {"Б/Ф", "Ф/С"};

    m_display.clear();
    m_display.setScale(1);
    m_display.home();
    m_display.print(info.name);

    if(questionsMode)
    {
      m_display.setCursor(80, 0);
      if(info.customInt == 1)
      {
        m_display.print("ОБЫЧНЫЙ");
      }
      else
      {
        m_display.print("БЛИЦ ");
        m_display.print(info.customInt);
      }
    }
    else
    {
      m_display.setCursor(110, 0);
      m_display.print(modeNames[static_cast<int>(info.mode)]);
    }

    if (m_soundMode == HalSoundMode::Disabled) {
      m_display.setCursor(38, 7);
      m_display.print("без звука");
    }
  }

  m_display.setScale(4);
  m_display.setCursor(0, 3);
  m_display.print("        ");
  showTime(info);
  m_display.update();

  m_displayForceUpdate = false;
}

void HalImpl::updateDisplay(const CustomDisplayInfo& info)
{
  if(info.type == DisplayInfoSettings)
  {
    updateDisplay((settings::Settings*)info.data);
    return;
  }
  if(info.type == DisplayInfoEightButtons)
  {
    m_state.display_mode = DisplayMode::EightButtons;

    m_display.clear();
    m_display.setScale(2);
    m_display.setCursor(16, 1);
    m_display.print("8 КНОПОК");
    m_display.setScale(1);
    m_display.setCursor(6, 4);
    m_display.print("управляйте с другой");
    m_display.setCursor(40, 6);
    m_display.print("системы");
  }
}

void HalImpl::updateDisplay(const settings::Settings* settings)
{
  m_state.display_mode = DisplayMode::Settings;

  m_display.clear();
  m_display.setScale(1);
  m_display.home();
  m_display.print("НАСТРОЙКИ");

  m_display.setScale(2);
  m_display.setCursor(0, 3);
  m_display.print(settings->getCurrentItem().getName());

  m_display.setScale(1);
  m_display.setCursor(0, 6);
  m_display.print(settings->getCurrentItem().getValueStr());
  m_display.update();
}

unsigned long HalImpl::getTimeMillis()
{
  return millis();
}

void HalImpl::saveSettings(const settings::Settings& settings)
{
  int data[settings.size()];
  settings.dumpData(data);

  for(int i=0; i<settings.size(); i++)
  {
    EEPROM.put(i * sizeof(int), data[i]);
  }
}

void HalImpl::loadSettings(settings::Settings& settings)
{
  int data[settings.size()];

  for(int i=0; i<settings.size(); i++)
  {
    EEPROM.get(i * sizeof(int), data[i]);
  }

  settings.loadData(data);
}

void HalImpl::setLinkVersion(link::UartLinkVersion version)
{
  if(m_link.getVersion() != version)
  {
    m_link = link::ArduinoUartLink(&Serial, version);
  }
}

void HalImpl::sendLinkCommand(bool useLink, link::Command command, unsigned int data)
{
  if(!useLink)
  {
    return;
  }

  m_link.send(command, data);
}

link::Link& HalImpl::getLink()
{
  return m_link;
}
