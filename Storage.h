#pragma once

struct State
{
  int gameNumber = 1;
  bool isFalstartEnabled = false;
  bool isSoundOn = false;
};

class Storage
{
  public:
    static void saveState(State state);
    static State getState();
};