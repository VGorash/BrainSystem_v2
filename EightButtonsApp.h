#ifndef EIGHT_BUTTONS_APP_H
#define EIGHT_BUTTONS_APP_H

#include "src/Framework/Core/App.h"

namespace vgs
{

class EightButtonsApp : public IApp
{
public:
  EightButtonsApp();

  void init(IHal& hal) override;
  void tick(IHal& hal) override;

  AppChangeType appChangeNeeded() override;
  IApp* createCustomApp() override;

private:
  bool m_shouldClose = false;
};

} // namespace vgs

#endif
