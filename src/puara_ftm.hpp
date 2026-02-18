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

  struct ftm_report_info
  {
    uint32_t rtt_est; // in nanoseconds
    uint32_t dist_est; // in centimeters
    int8_t rssi; // RSSI of the FTM frame, as an indicator of signal strength of the FTM responder AP
  };

   /**
    * Configure and trigger an FTM procedure, with parameters defined in wifi_ftm_cfg. Get the FTM report data in the event handler (sta_event_handler in WiFi module) when the procedure is completed. Call this only on devices acting as FTM initiators (STAs connected to a responder AP). Do NOT call this on devices acting as solely as FTM responders (APs).
    */
// as the objective is trilateration, qudrilateration, or more, users should have a struct for ftm configurations which is a definable array in the setup loop
  std::vector<scanned_responder_ap_info> scanned_responder_aps; // Array to hold scanned AP info for FTM purposes, populated during WiFi scan

  ftm_report_info last_ftm_report{}; // struct to hold the last FTM report data (RTT, distance, RSSI)  
  
  bool ftm_report_available = false; // flag to indicate if FTM report data is available  

  std::vector<wifi_ftm_initiator_cfg_t> wifi_ftm_cfgs; // Array to hold FTM configuration for multiple APs, to be defined by user before calling requestFTM()

  // take in scanned_responder_aps and create wifi_ftm_cfgs vector based on user-defined parameters for frame count and burst period (same for all APs for now but can be made more flexible in the future) - this is to be called in setup after wifi_scan and before requestFTM
  std::vector<wifi_ftm_initiator_cfg_t> make_FTM_Configuration_Vectors(
    const std::vector<PuaraAPI::FTM::scanned_responder_ap_info>& responders,  
    uint8_t frame_count, 
    uint16_t burst_period);


};
} 
