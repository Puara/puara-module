#pragma once

#include "esp_log.h"
#include <esp_wifi.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

namespace PuaraAPI
{
//struct WiFi;

struct FTM
{
    // flag to indicate if FTM report data is available  
  bool ftm_report_available = false;   //API accessible 

  struct scanned_responder_ap_info
  {
    std::string ssid;
    uint8_t bssid[6];
    int8_t rssi;
    uint8_t primary_channel;
  };

  // Vector to hold scanned found Responder-Ready APs, populated during WiFi scan
  std::vector<scanned_responder_ap_info> scanned_responder_aps{};    //internal only - No API calls


  struct ftm_report_info
  {
    uint32_t rtt_est; // in nanoseconds
    uint32_t dist_est; // in centimeters
    int8_t rssi; // RSSI of the FTM frame, as an indicator of signal strength of the FTM responder AP
  };

  ftm_report_info last_ftm_report{};  // API accessible 


  // Map of user-defined FTM responder configurations, with SSID as key and 
  // wifi_ftm_initiator_cfg_t struct (holding responder MAC, channel, frame count,
  // burst period) as value
  std::map<std::string, wifi_ftm_initiator_cfg_t> map_of_responder_configs{}; // API accessible 


  // Return a map of responders defined by user and their wifi_ftm_cfg_t struct info
  // (MAC, channel, frame count, burst period) for upcoming FTM session initiation.
  // `returned_ssids` is passed by const-reference to avoid copying.
  const std::map<std::string, wifi_ftm_initiator_cfg_t> get_map_of_responder_configs(
    uint8_t frame_count, uint16_t burst_period, const std::vector<std::string>& returned_ssids); 



};
} 