#include "src/Framework/Core/Engine.h"

#include "HalImpl.h"
#include "SettingsApp.h"

using namespace vgs;

Engine engine(new HalImpl(), nullptr, new SettingsApp(true));

void setup() {
  engine.init();
}

void loop() {
  engine.tick();
}
