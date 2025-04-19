#ifndef EIGHT_BUTTONS_APP_H
#define EIGHT_BUTTONS_APP_H

#include "src/Framework/Core/App.h"

namespace vgs
{

class EightButtonsApp : public App
{
public:
  EightButtonsApp();

  void init(Hal* hal) override;
  void tick(Hal* hal) override;

  AppChangeType appChangeNeeded() override;
  App* getCustomApp() override;

private:
  bool m_shouldClose = false;
  
};

} // namespace vgs

#endif
