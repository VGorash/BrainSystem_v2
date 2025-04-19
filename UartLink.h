#ifndef UART_LINK_H
#define UART_LINK_H

#include "Link.h"

#define UART_LINK_MAX_PLAYERS 16

namespace vgs::link
{   

  enum class UartLinkVersion
  {
    V1, // outdated version. No display commands. Differece in player numbers (see commands in cpp file)
    V2, // current version
  };

  // UART parameters: 9600 baud, 8-N-1
  class UartLink : public Link
  {
  public:
    UartLink(UartLinkVersion version);

    UartLinkVersion getVersion();

    void tick() override;
    Command getCommand() override;
    unsigned int getData() override;
    void send(Command command, unsigned int data = 0) override;

  protected:
    virtual void writeUartData(char data) = 0; // write 1 byte, platform-specific
    virtual bool readUartData(char& data) = 0; // read 1 byte if available and return true if byte has been read, platform-specific

    void tickV1();
    void tickV2();

    void sendV1(Command command, unsigned int data);
    void sendV2(Command command, unsigned int data);

  protected:
    UartLinkVersion m_version;

    Command m_command = Command::None;
    unsigned int m_data = 0;
  };

} // namespace vgs::link

#endif