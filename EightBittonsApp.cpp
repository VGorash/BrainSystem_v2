#include "EightButtonsApp.h"
#include "HalImpl.h"

using namespace vgs;

EightButtonsApp::EightButtonsApp()
{

}

void EightButtonsApp::init(IHal& hal)
{
  CustomDisplayInfo info;
  info.name = "eight_buttons";
  hal.updateDisplay(info);
}

void EightButtonsApp::tick(IHal& hal)
{
  HalImpl* halImpl = (HalImpl*) &hal;
  link::Link& linkHandle = halImpl->getLink();
  ButtonState buttonState = halImpl->getButtonState();

  if(buttonState.menu)
  {
    m_shouldClose = true;
    return;
  }

  switch(linkHandle.getCommand())
  {
    case link::Command::PlayerLedOn:
    {
      halImpl->playerLedOn(linkHandle.getData(), false);
    }
    break;

    case link::Command::PlayerLedBlink:
    {
      halImpl->playerLedBlink(linkHandle.getData(), false);
    }
    break;

    case link::Command::SignalLedOn:
    {
      halImpl->signalLedOn(false);
    }
    break;

    case link::Command::LedsOff:
    {
      halImpl->ledsOff(false);
    }
    break;
  }

  if(buttonState.player >= 0 && buttonState.player < NUM_PLAYERS)
  {
    linkHandle.send(link::Command::PlayerButton, buttonState.player);
  }
}

AppChangeType EightButtonsApp::appChangeNeeded()
{
  if(m_shouldClose)
  {
    return AppChangeType::Menu;
  }

  return AppChangeType::None;
}

IApp* EightButtonsApp::createCustomApp()
{
  return nullptr;
}
