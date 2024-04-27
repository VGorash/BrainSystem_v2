#include "Game.h"
#include <Arduino.h>

EightButtonsGame::EightButtonsGame(bool isFalstartEnabled, Display& display) : Game(isFalstartEnabled, display)
{
}

void EightButtonsGame::onPlayerButtonPress(int player)
{
  sendUartData(UART_SLAVE_PLAYER_BUTTON_PRESSED, player);  
}

void EightButtonsGame::onStartButtonPress(){}

void EightButtonsGame::onStopButtonPress(){}

const char* EightButtonsGame::getName()
{
  return "8 КНОПОК";
}

void EightButtonsGame::onUartDataReceive(byte data)
{
  byte command = data & 0xF0;
  byte payload = data & 0x0F;

  switch(command){
    case UART_SLAVE_CLEANUP:
    {
      cleanup();
      break;
    }
    case UART_SLAVE_ENABLE_LED:
    {
      if(payload >= 4)
      {
        digitalWrite(leds[payload - 4], 1);
      }
      break;
    }
    case UART_SLAVE_BLINK_LED:
    {
      if(payload >= 4)
      {
        blinkLed(leds[payload - 4]);
      }
      break;
    }
    case UART_SLAVE_ENABLE_SIGNAL:
    {
      digitalWrite(LED_SIGNAL, 1);
      break;
    }
  }
}