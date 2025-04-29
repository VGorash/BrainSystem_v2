#include "SettingsApp.h"
#include "HalImpl.h"

#include "src/Framework/Game.h"
#include "src/Framework/JeopardyGame.h"
#include "src/Framework/BrainRingGame.h"
#include "EightButtonsApp.h"

using namespace vgs;

struct GameInfo
{
  App* (*constructor)(const GameConfig&);
};

App* createGame(const GameConfig& config)
{
  return new Game(config);
}

App* createJeopardyGame(const GameConfig& config)
{
  return new JeopardyGame(config);
}

App* createBrainRingGame(const GameConfig& config)
{
  return new BrainRingGame(config);
}

App* createEightButtonsApp(const GameConfig& config)
{
  return new EightButtonsApp();
}

typedef App* (*GameConstructor)(const GameConfig&);

constexpr int gameCount = 4;
constexpr GameConstructor gameConstructors[gameCount] = {createGame, createJeopardyGame, createBrainRingGame, createEightButtonsApp};

constexpr const char* gameNames[gameCount] = {"Без отсчета", "Своя игра", "Брейн-ринг", "8 кнопок"};
constexpr const char* modeNames[2] = {"Без фальстартов", "С фальстартами"};
constexpr const char* onOffNames[2] = {"Включен", "Выключен"};
constexpr const char* linkModes[2] = {"V1 (устаревший)", "V2 (обычный)"};

SettingsApp::SettingsApp(bool launchGame) : m_launchGame(launchGame)
{
  m_settings.addItem(new ListSettingsItem("Тип игры", gameCount, gameNames));  
  m_settings.addItem(new ListSettingsItem("Режим", 2, modeNames));
  m_settings.addItem(new ListSettingsItem("Звук", 2, onOffNames));
  m_settings.addItem(new ListSettingsItem("Свет", 2, onOffNames));
  m_settings.addItem(new ListSettingsItem("Link", 2, linkModes));
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
  int linkVersion = settingsState[4];

  halImpl->setSoundMode(static_cast<HalSoundMode>(soundMode));
  halImpl->setSignalLightEnabled(!signalLightDisabled);
  halImpl->setLinkVersion(static_cast<vgs::link::UartLinkVersion>(linkVersion));

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
  
  GameConfig config;
  config.falstartEnabled = (bool) settingsState[1];

  return gameConstructors[gameNumber](config);
}