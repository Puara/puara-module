#pragma once

#include "esp_log.h"
#include <esp_wifi.h>

#include <iomanip>
#include <iostream>
#include <string>

namespace PuaraAPI
{

struct WiFi;

struct FTM
{
  WiFi& wifi;

  struct scanned_ap_info
  {
    std::string ssid;
    uint8_t bssid[6];
    int8_t rssi;
    uint8_t primary_channel;
  };

  scanned_ap_info scanned_aps[20] = {}; // Array to hold scanned AP info for FTM purposes, populated during WiFi scan

  //std::string externalAP_BSSID; //used for FTM proof
  uint8_t externalAP_BSSID[6];    // MAC address of router
  uint8_t ftm_channel; // channel of AP for FTM purposes
  uint32_t ftm_responder_state; // flag to verify if external AP is an FTM responder
  uint32_t ftm_initiator_state; // flag to verify if external responder is also initiator
  uint32_t last_distance_cm = 0; // last distance estimate in cm from FTM
  uint32_t last_rtt_ns = 0; // last RTT estimate in ns from FTM
  bool ftm_report_available = false; // flag to indicate if FTM report data is available  
  int rssi_of_ftm_frame = 0;

  wifi_ftm_initiator_cfg_t wifi_ftm_cfg{};

  void configureFTM(uint8_t frm_cnt = 32, uint16_t burst_prd = 4); // Configure FTM parameters
  void requestFTM(); 
  bool set_offset_responder(int16_t offset_cm); // Set offset in cm for FTM Responder
  void end_ftm_request_session(); 
};
} 
