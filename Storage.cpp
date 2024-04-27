#include "Storage.h"
#include <EEPROM.h>

#define INIT_ADDR 1023
#define INIT_KEY 0 

Storage::Storage()
{
  if (EEPROM.read(INIT_ADDR) != INIT_KEY) {
    EEPROM.write(INIT_ADDR, INIT_KEY);
  }
  else
  {
    EEPROM.get(0, m_state);
  }
}

void Storage::setState(const State& newState)
{
  m_state = newState;
  EEPROM.put(0, m_state);
}

State Storage::getState()
{
  return m_state;
}