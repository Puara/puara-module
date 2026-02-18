#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>

#include <esp_wifi.h>

#include <string>

namespace PuaraAPI
{
struct DeviceConfiguration;

struct FTM;

struct WiFi
{
  DeviceConfiguration& config;
  FTM* ftm = nullptr;

  // Public API
  void start_wifi();
  void wifi_scan(void);
  bool get_StaIsConnected();

  // Private API, accessed by web though
  bool StaIsConnected = false;
  bool ApStarted = false;

  std::string currentSSID;
  std::string currentSTA_IP;
  std::string currentSTA_MAC;
  std::string currentAP_IP;
  std::string currentAP_MAC;

  std::string wifiAvailableSsid;

  // Internal API      //"sta" in sta_event_handler creates confusion and function name should be cleaned
  static void sta_event_handler(
      void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
  void wifi_init();

  const int wifi_connected_bit = BIT0;
  const int wifi_fail_bit = BIT1;
  EventGroupHandle_t s_wifi_event_group = NULL;

  wifi_config_t wifi_config_sta;
  wifi_config_t wifi_config_ap;
  short int connect_counter;
};
} 