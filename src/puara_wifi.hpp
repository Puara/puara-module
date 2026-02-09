#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>

#include <esp_wifi.h>

#include <string>

namespace PuaraAPI
{
struct DeviceConfiguration;

struct WiFi
{
  DeviceConfiguration& config;

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

  //FTM related variables
  std::string router_BSSID; //used for FTM proof
  uint8_t currentRouter_BSSID[6];    // MAC address of router
  uint8_t ftm_channel; // channel of AP for FTM purposes
  uint32_t ftm_responder_state; // flag to verify if connected responder supports FTM 
  uint32_t ftm_initiator_state; // flag to verify if connected responder supports FTM 
  uint32_t last_distance_cm = 0; // last distance estimate in cm from FTM
  uint32_t last_rtt_ns = 0; // last RTT estimate in ns from FTM
  bool ftm_report_available = false; // flag to indicate if FTM report data is available  
  // end FTM related variables

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