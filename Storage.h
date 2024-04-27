#pragma once

struct State
{
  int gameNumber = 0;
  bool isFalstartEnabled = false;
  bool isSoundOn = false;
};

class Storage
{
  public:
    Storage();

    void setState(const State& newState);
    State getState();

  private:
    State m_state;
};