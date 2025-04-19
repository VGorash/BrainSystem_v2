#ifdef USE_ARDUINO_UART_LINK

#ifndef ARDUINO_UART_LINK_H
#define ARDUINO_UART_LINK_H

#include <Arduino.h>

#include "UartLink.h"

namespace vgs::link
{
  class ArduinoUartLink : public UartLink
  {
  public:
    ArduinoUartLink(HardwareSerial* serial, UartLinkVersion version = UartLinkVersion::V2) : UartLink(version), m_serial(serial)
    {

    }

  protected:
    void writeUartData(char data) override
    {
      m_serial->write(data);
    }

    bool readUartData(char& data) override
    {
      if(m_serial->available())
      {
        int raw = Serial.read();

        if(raw == -1)
        {
          return false;
        }

        data = (char)raw;

        return true;
      }

      return false;
    }

  protected:
    HardwareSerial* m_serial;

  };

} //namespace vgs::link


#endif

#endif
