#pragma once

class Runnable
{
  public:
    virtual ~Runnable() = default;

    virtual void tick(){};

    virtual void onStartButtonPress(){};
    virtual void onStopButtonPress(){};
    virtual void onFuncButtonPress(){};
    virtual void onPlayerButtonPress(int player){};
    virtual void onUartDataReceive(byte data){};

    virtual const char* getName() = 0;
};