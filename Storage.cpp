#include "Storage.h"
#include <EEPROM.h>

#define INIT_ADDR 1023
#define INIT_KEY 0

void Storage::saveState(State state)
{
  EEPROM.put(0, state);
}

State Storage::getState()
{
  State state;
  if (EEPROM.read(INIT_ADDR) != INIT_KEY) {
    EEPROM.write(INIT_ADDR, INIT_KEY);
    EEPROM.put(0, state);
  }
  else
  {
    EEPROM.get(0, state);
  }
  return state;
}