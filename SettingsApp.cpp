#include "SettingsApp.h"
#include "HalImpl.h"

#include "src/Framework/Game.h"
#include "src/Framework/JeopardyGame.h"
#include "src/Framework/BrainRingGame.h"

using namespace vgs;

struct GameInfo
{
  const char* name;
  App* (*constructor)(bool);
};

App* createGame(bool falstartEnabled)
{
  return new Game(falstartEnabled);
}

App* createJeopardyGame(bool falstartEnabled)
{
  return new JeopardyGame(falstartEnabled);
}

App* createBrainRingGame(bool falstartEnabled)
{
  return new BrainRingGame(falstartEnabled);
}

constexpr int gameCount = 3;
constexpr GameInfo games[gameCount] = {
  {"Без отсчета", createGame},
  {"Своя игра", createJeopardyGame},
  {"Брейн-ринг", createBrainRingGame}
};

SettingsApp::SettingsApp(bool launchGame) : m_launchGame(launchGame)
{
  const char* gameNames[gameCount];
  for(int i=0; i<gameCount; i++)
  {
    gameNames[i] = games[i].name;
  }
  m_settings.addItem("Тип игры", gameCount, gameNames);

  const char* modeNames[2] = {"Без фальстартов", "С фальстартами"};
  m_settings.addItem("Режим", 2, modeNames);

  const char* onOffNames[2] = {"Включен", "Выключен"};
  m_settings.addItem("Звук", 2, onOffNames);
  m_settings.addItem("Свет", 2, onOffNames);
}

void SettingsApp::init(Hal* hal)
{
  hal->loadSettings(m_settings);

  if(m_launchGame)
  {
    m_launchGame = false;
    exit(hal);
  }
}

void SettingsApp::tick(Hal* hal)
{
  if(m_shouldClose)
  {
    return;
  }

  process(hal);

  if(m_displayDirty)
  {
    SettingsDisplayInfo info;
    info.settings = &m_settings;
    hal->updateDisplay(info);
    m_displayDirty = false;
  }
}

void SettingsApp::process(Hal* hal)
{
  ButtonState buttonState = hal->getButtonState();

  if(buttonState.menu)
  {
    exit(hal);
    return;
  }
  if(buttonState.enter)
  {
    m_settings.next();
    m_displayDirty = true;
    return;
  }
  if(buttonState.start)
  {
    m_settings.getCurrentItem()->increment();
    m_displayDirty = true;
    m_settingsDirty = true;
    return;
  }
  if(buttonState.stop)
  {
    m_settings.getCurrentItem()->decrement();
    m_displayDirty = true;
    m_settingsDirty = true;
    return;
  }
}

void SettingsApp::exit(Hal* hal)
{
  HalImpl* halImpl = (HalImpl*) hal;

  if(m_settingsDirty)
  {
    halImpl->saveSettings(m_settings);
    m_settingsDirty = false;
  }

  int settingsState[m_settings.size()];
  m_settings.dumpData(settingsState);
  int soundMode = settingsState[2];
  bool signalLightDisabled = settingsState[3];

  halImpl->setSoundMode(static_cast<HalSoundMode>(soundMode));
  halImpl->setSignalLightEnabled(!signalLightDisabled);

  m_shouldClose = true;
}

AppChangeType SettingsApp::appChangeNeeded()
{
  if(m_shouldClose)
  {
    m_displayDirty = true;
    m_shouldClose = false;
    return AppChangeType::Custom;
  }

  return AppChangeType::None;
}

App* SettingsApp::getCustomApp()
{
  int settingsState[m_settings.size()];
  m_settings.dumpData(settingsState);

  int gameNumber = settingsState[0];
  bool falstartEnabled = (bool) settingsState[1];
  int soundEnabled = settingsState[2];

  return games[gameNumber].constructor(falstartEnabled);
}