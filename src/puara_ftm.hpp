#pragma once

#include "esp_log.h"
#include <esp_wifi.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace PuaraAPI
{

struct WiFi;

struct FTM
{
//  WiFi& wifi;

// before going further, make this struct available through API and try to see if ftm config / request functional

  struct scanned_responder_ap_info
  {
    std::string ssid;
    uint8_t bssid[6];
    int8_t rssi;
    uint8_t primary_channel;
  };

// as the objective is trilateration, qudrilateration, or more, users should have a struct for ftm configurations which is a definable array in the setup loop

  std::vector<scanned_responder_ap_info> scanned_responder_aps; // Array to hold scanned AP info for FTM purposes, populated during WiFi scan

  uint32_t last_distance_cm = 0; // last distance estimate in cm from FTM
  uint32_t last_rtt_ns = 0; // last RTT estimate in ns from FTM
  bool ftm_report_available = false; // flag to indicate if FTM report data is available  
  int rssi_of_ftm_frame = 0;

  wifi_ftm_initiator_cfg_t wifi_ftm_cfg{};

  /*
//  void configureFTM(uint8_t frm_cnt = 32, uint16_t burst_prd = 4); // Configure FTM parameters
  void requestFTM(); 
  bool set_offset_responder(int16_t offset_cm); // Set offset in cm for FTM Responder
  void end_ftm_request_session(); 
  */
};
} 
