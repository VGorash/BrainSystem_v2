#include "SettingsApp.h"
#include "HalImpl.h"

#include "src/Framework/Game.h"
#include "src/Framework/JeopardyGame.h"
#include "src/Framework/BrainRingGame.h"
#include "EightButtonsApp.h"

using namespace vgs;

struct GameInfo
{
  IApp* (*constructor)(const GameConfig&);
};

IApp* createGame(const GameConfig& config)
{
  return new Game(config);
}

IApp* createJeopardyGame(const GameConfig& config)
{
  return new JeopardyGame(config);
}

IApp* createBrainRingGame(const GameConfig& config)
{
  return new BrainRingGame(config);
}

IApp* createEightButtonsApp(const GameConfig& config)
{
  return new EightButtonsApp();
}

typedef IApp* (*GameConstructor)(const GameConfig&);

constexpr int gameCount = 4;
constexpr GameConstructor gameConstructors[gameCount] = {createGame, createJeopardyGame, createBrainRingGame, createEightButtonsApp};

constexpr const char* gameNames[gameCount] = {"БЕЗ ОТСЧЕТА", "СВОЯ ИГРА", "БРЕЙН-РИНГ", "8 КНОПОК"};
constexpr const char* modeNames[2] = {"БЕЗ ФАЛЬСТАРТОВ", "С ФАЛЬСТАРТАМИ"};
constexpr const char* onOffNames[2] = {"ВКЛ", "ВЫКЛ"};
constexpr const char* linkModes[2] = {"V1 (устаревший)", "V2 (обычный)"};

SettingsApp::SettingsApp(bool launchGame) : m_launchGame(launchGame)
{
  m_settings.addItem(new ListSettingsItem("Тип игры", gameCount, gameNames));  
  m_settings.addItem(new ListSettingsItem("Режим", 2, modeNames));
  m_settings.addItem(new ListSettingsItem("Звук", 2, onOffNames));
  m_settings.addItem(new ListSettingsItem("Свет", 2, onOffNames));
  m_settings.addItem(new ListSettingsItem("Link", 2, linkModes));
}

void SettingsApp::init(IHal& hal)
{
  hal.loadSettings(m_settings);

  if(m_launchGame)
  {
    m_launchGame = false;
    exit(hal);
  }
}

void SettingsApp::tick(IHal& hal)
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
    hal.updateDisplay(info);
    m_displayDirty = false;
  }
}

void SettingsApp::process(IHal& hal)
{
  ButtonState buttonState = hal.getButtonState();

  if(buttonState.menu)
  {
    exit(hal);
    return;
  }
  if(buttonState.enter)
  {
    m_settings.moveNext();
    m_displayDirty = true;
    return;
  }
  if(buttonState.start)
  {
    m_settings.getCurrentItem().increment();
    m_displayDirty = true;
    m_settingsDirty = true;
    return;
  }
  if(buttonState.stop)
  {
    m_settings.getCurrentItem().decrement();
    m_displayDirty = true;
    m_settingsDirty = true;
    return;
  }
}

void SettingsApp::exit(IHal& hal)
{
  HalImpl* halImpl = (HalImpl*) &hal;

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

IApp* SettingsApp::createCustomApp()
{
  int settingsState[m_settings.size()];
  m_settings.dumpData(settingsState);

  int gameNumber = settingsState[0];
  
  GameConfig config;
  config.displayed_name = gameNames[gameNumber];
  config.mode = static_cast<GameMode>(settingsState[1]);

  return gameConstructors[gameNumber](config);
}