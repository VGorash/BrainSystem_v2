#pragma once

#include "display.h"

#include "Runnable.h"
#include "Storage.h"
#include "SettingsItem.h"

class SettingsWindow : public Runnable
{
  public:
    SettingsWindow(State state, Display& display);
    ~SettingsWindow();

    void onStartButtonPress() override;
    void onStopButtonPress() override;
    void onFuncButtonPress() override;

    const char* getName() override;

  private:
    void updateDisplayState();

  private:
    Display& m_display;
    State m_state;

    int m_currentSetting = 0;
    int m_numSettings;
    SettingsItem** m_settings;
};

