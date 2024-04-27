class Runnable
{
  public:
    virtual void tick();

    virtual void onStartButtonPress(){};
    virtual void onStopButtonPress(){};
    virtual void onPlayerButtonPress(int player){};
    virtual void onUartDataReceive(byte data){};

    virtual void switchSound(){};
};