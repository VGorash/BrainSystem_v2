#ifndef HAL_IMPL_H
#define HAL_IMPL_H

#define USE_ARDUINO_UART_LINK

#include <EncButton.h>

#include "src/Framework/Core/Hal.h"
#include "src/Framework/Timer.h"
#include "src/Link/ArduinoUartLink.h"

#include "Display.h"


#define NUM_PLAYERS 4

enum class HalSoundMode
{
  Normal,
  Disabled
};

enum class DisplayMode
{
  Game,
  Settings, 
  EightButtons
};

struct HalState
{
  DisplayMode display_mode;
};

class HalImpl : public vgs::IHal
{
public:
  HalImpl();
  ~HalImpl();

  void init() override;
  void tick() override;

  //buttons
  vgs::ButtonState getButtonState() override;

  //leds
  void playerLedOn(int player) override;
  void playerLedOn(int player, bool useLink);
  void playerLedBlink(int player) override;
  void playerLedBlink(int player, bool useLink);
  void signalLedOn() override;
  void signalLedOn(bool useLink);
  void ledsOff() override;
  void ledsOff(bool useLink);
  void setSignalLightEnabled(bool enabled);

  //sound
  void sound(vgs::HalSound soundType) override;
  void sound(unsigned int frequency, unsigned int duration) override;
  void setSoundMode(HalSoundMode mode);

  //display
  void updateDisplay(const vgs::GameDisplayInfo& info) override;
  void updateDisplay(const vgs::SettingsDisplayInfo& info) override;
  void updateDisplay(const vgs::CustomDisplayInfo& info) override;

  //time
  unsigned long getTimeMillis() override;

  //settings
  void saveSettings(const vgs::ISettings& settings) override;
  void loadSettings(vgs::ISettings& settings) override;

  // link
  void setLinkVersion(vgs::link::UartLinkVersion);
  vgs::link::Link* getLink();

private:
  void showTime(const vgs::GameDisplayInfo& info);
  void showPressTime(const vgs::GameDisplayInfo& info);

  void sendLinkCommand(bool useLink, vgs::link::Command command, unsigned int data = 0);

private:  
  Button m_playerButtons[NUM_PLAYERS];
  Button m_startButton;
  Button m_stopButton;
  Button m_funcButton;

  vgs::Timer m_blinkTimer;
  bool m_blinkState = 0;
  bool m_blinkingLeds[NUM_PLAYERS];

  vgs::link::UartLink* m_link;

  HalSoundMode m_soundMode;
  bool m_signalLightEnabled = true;

  Display m_display;
  bool m_displayForceUpdate = true;

  HalState m_state;
};

#endif
