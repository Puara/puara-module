//****************************************************************************//
// Puara Module Manager - WiFi and file system functions                      //
// Metalab - Société des Arts Technologiques (SAT)                            //
// Input Devices and Music Interaction Laboratory (IDMIL), McGill University  //
// Edu Meneses (2022) - https://www.edumeneses.com                            //
//****************************************************************************//

#ifndef PUARA_H
#define PUARA_H

#include "puara_config.hpp"

#include <string>
#include <string_view>

typedef void* httpd_handle_t;
class Puara
{
public:
  void start(PuaraAPI::Monitors monitor = PuaraAPI::UART_MONITOR);

  httpd_handle_t start_webserver(void);
  void stop_webserver(void);

  std::string get_dmi_name();
  unsigned int get_version();
  void set_version(unsigned int user_version);
  std::string getIP1();
  std::string getIP2();
  bool IP1_ready();
  bool IP2_ready();
  int unsigned getPORT1();
  int unsigned getPORT2();
  std::string getPORT1Str();
  std::string getPORT2Str();
  int unsigned getLocalPORT();
  std::string getLocalPORTStr();

  void mount();
  void unmount();

  void read_config_json();
  void write_config_json();
  void read_settings_json();
  void write_settings_json();

  bool start_serial_listening();
  void send_serial_data(std::string data);

  void start_mdns_service(std::string_view device_name, std::string_view instance_name);

  void start_wifi();
  void wifi_scan(void);
  bool get_StaIsConnected();

  double getVarNumber(std::string varName);
  std::string getVarText(std::string varName);
};

#endif
