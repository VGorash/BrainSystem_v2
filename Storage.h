#pragma once

struct State
{
  int gameNumber = 1;
  bool isFalstartEnabled = false;
  int soundMode = 1;
};

class Storage
{
  public:
    static void saveState(State state);
    static State getState();
};