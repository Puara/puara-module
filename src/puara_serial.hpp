#pragma once
#include "puara_config.hpp"

#define PUARA_SERIAL_BUFSIZE 1024

namespace PuaraAPI
{
struct DeviceConfiguration;
struct Device;
struct SPIFFS;
struct SpiffsJSONSettings;

struct Serial
{
  DeviceConfiguration& config;
  Device& device;
  SPIFFS& spiffs;
  SpiffsJSONSettings& settings;

  // Set default monitor as UART
  int module_monitor = PuaraAPI::UART_MONITOR;

  // public API
  bool start_serial_listening();
  void send_serial_data(std::string data);

  // private API
  void interpret_serial();
  void uart_monitor();
  void jtag_monitor();
  void usb_monitor();

  char serial_data[PUARA_SERIAL_BUFSIZE];
  int serial_data_length;
  std::string serial_data_str;
  std::string serial_data_str_buffer;
};
}