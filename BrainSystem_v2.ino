#include "src/Framework/Core/Engine.h"

#include "HalImpl.h"
#include "SettingsApp.h"

using namespace vgs;

HalImpl* hal;
Engine* engine;

App* initSettings()
{
  return new SettingsApp(true);
}

void setup() {
  Serial.begin(9600);
  Wire.setClock(800000L);
  delay(100);

  hal = new HalImpl();
  hal->init();

  engine = new Engine(hal, nullptr, initSettings);
}

void loop() {
  engine->tick();
}


