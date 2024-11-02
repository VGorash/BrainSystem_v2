#pragma once

#include "Storage.h"

class SettingsItem
{
  public:
    SettingsItem(int size, const char* name, const char* values[], State& state, void(*updateStateFunc)(State&, int), int initialValue);
    ~SettingsItem();
  
    void increment();
    void decrement();

    const char* getName(); 
    const char* getCurrentValue();
  
  private:
    const char** m_values;
    const char* m_name;
    const int m_size;
    int m_currentItem;

    void(*m_updateStateFunc)(State&, int);
    State& m_state;
};