#include "SettingsWindow.h"


SettingsWindow::SettingsWindow(State state, Display& display): m_display(display), m_state(state)
{
  m_numSettings = 3;
  m_settings = (SettingsItem**) malloc(m_numSettings * sizeof(SettingsItem*));

  const char* game_types[4] = {"без отсчета", "своя игра", "брейн-ринг", "8 кнопок"};
  m_settings[0] = new SettingsItem(4, "Тип игры", game_types, m_state, [](State& s, int v){s.gameNumber = v;}, m_state.gameNumber);

  const char* onOffModes[2] = {"выкл", "вкл"};
  m_settings[1] = new SettingsItem(2, "Фальстарты", onOffModes, m_state, [](State& s, int v){s.isFalstartEnabled = (bool) v;}, m_state.isFalstartEnabled);

  const char* soundModes[3] = {"без звука", "обычный", "громко"};
  m_settings[2] = new SettingsItem(3, "Тип звука", soundModes, m_state, [](State& s, int v){s.soundMode =  v;}, m_state.soundMode);

  updateDisplayState();
}

SettingsWindow::~SettingsWindow()
{
  for(int i=0; i<m_numSettings; i++)
  {
    delete m_settings[i];
  }
  free(m_settings);
  Storage::saveState(m_state);
}

void SettingsWindow::onStartButtonPress()
{
  m_settings[m_currentSetting]->increment();
  updateDisplayState();
}

void SettingsWindow::onStopButtonPress()
{
  m_settings[m_currentSetting]->decrement();
  updateDisplayState();
}

void SettingsWindow::onFuncButtonPress()
{
  m_currentSetting = (m_currentSetting + 1) % m_numSettings;
  updateDisplayState();
}

void SettingsWindow::updateDisplayState()
{
  m_display.clear();
  m_display.setScale(1);
  m_display.home();
  m_display.print(getName());

  m_display.setScale(2);
  m_display.setCursor(0, 3);
  m_display.print(m_settings[m_currentSetting]->getName());

  m_display.setScale(1);
  m_display.setCursor(0, 6);
  m_display.print(m_settings[m_currentSetting]->getCurrentValue());
  m_display.update();
}

const char* SettingsWindow::getName()
{
  return "НАСТРОЙКИ";
}