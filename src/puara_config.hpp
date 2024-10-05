#pragma once
#include <string>

namespace PuaraAPI {

enum Monitors { UART_MONITOR = 0, JTAG_MONITOR = 1, USB_MONITOR = 2 };

struct DeviceConfiguration {
  unsigned int version = 20220906;
  std::string dmiName;
  std::string device;
  unsigned int id;
  std::string author;
  std::string institution;
  std::string APpasswd;
  std::string wifiSSID;
  std::string wifiPSK;
  bool persistentAP = false;
  std::string oscIP1;
  unsigned int oscPORT1;
  std::string oscIP2;
  unsigned int oscPORT2;
  unsigned int localPORT;

  bool IP1_ready();
  bool IP2_ready();
};
}  // namespace PuaraAPI