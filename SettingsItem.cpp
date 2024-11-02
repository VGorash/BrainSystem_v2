#include "SettingsItem.h"

SettingsItem::SettingsItem(int size, const char* name, const char* values[], State& state, void(*updateStateFunc)(State&, int), int initialValue): m_size(size), m_name(name), m_state(state), m_updateStateFunc(updateStateFunc), m_currentItem(initialValue)
{
  m_values = new const char*[size];
  for(int i=0; i<size; i++)
  {
    m_values[i] = values[i];
  }
}

SettingsItem::~SettingsItem()
{
  delete[] m_values;
}

void SettingsItem::increment()
{
  m_currentItem++;
  if(m_currentItem == m_size)
  {
    m_currentItem = 0;
  }
  m_updateStateFunc(m_state, m_currentItem);
}

void SettingsItem::decrement()
{
  m_currentItem--;
  if(m_currentItem == -1)
  {
    m_currentItem = m_size - 1;
  }
  m_updateStateFunc(m_state, m_currentItem);
}

const char* SettingsItem::getName()
{
  return m_name;
}

const char* SettingsItem::getCurrentValue()
{
  return m_values[m_currentItem];
}