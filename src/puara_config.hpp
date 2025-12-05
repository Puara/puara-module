#pragma once
#include <string>
#include <iostream>

//#define PUARA_DEBUG  //uncomment this line to access all LOG statements for debug

#ifdef PUARA_DEBUG
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"

  #define LOG(x) do { \
    UBaseType_t stackHighWaterMark = uxTaskGetStackHighWaterMark(NULL); \
    if (stackHighWaterMark < 100) { \
        fprintf(stderr, "WARNING: Low stack space! Remaining: %lu\n", (unsigned long)stackHighWaterMark); \
    } \
    fprintf(stderr, "%s\n", x); \
  } while(0)
#else
  #define LOG(x) do {} while(0)
#endif

namespace PuaraAPI
{

enum Monitors
{
  UART_MONITOR = 0,
  JTAG_MONITOR = 1,
  USB_MONITOR = 2
};

struct DeviceConfiguration
{
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
};
} 