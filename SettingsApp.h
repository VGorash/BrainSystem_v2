#ifndef SETTINGS_APP_H
#define SETTINGS_APP_H

#include "src/Framework/Core/App.h"
#include "src/Framework/Core/Settings.h"

namespace vgs
{

class SettingsApp : public IApp
{
public:
  SettingsApp(bool launchGame);

  void init(IHal& hal) override;
  void tick(IHal& hal) override;

  AppChangeType appChangeNeeded() override;
  IApp* createCustomApp() override;

private:
  void process(IHal& hal);
  void exit(IHal& hal);

private:
  bool m_launchGame;
  
  bool m_displayDirty = true;
  bool m_settingsDirty = false;
  bool m_shouldClose = false;

  Settings m_settings;

};

} // namespace vgs

#endif