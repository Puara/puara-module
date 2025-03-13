#pragma once
#include <string>
#include <iostream>
#include <esp_wifi.h>
#include "esp_log.h"
#include <iomanip>

namespace PuaraAPI
{
struct WiFi;

struct FTM
{

WiFi& wifi;

  wifi_ftm_initiator_cfg_t wifi_ftm_cfg; //struct for ftm config
    /** FTM Initiator configuration
      typedef struct {
          uint8_t resp_mac[6];        // MAC address of the FTM Responder 
          uint8_t channel;            // Primary channel of the FTM Responder
          uint8_t frm_count;          // No. of FTM frames requested in terms of 4 or 8 bursts (allowed values - 0(No pref), 16, 24, 32, 64)
          uint16_t burst_period;      // Requested time period between consecutive FTM bursts in 100's of milliseconds (0 - No pref)
      } wifi_ftm_initiator_cfg_t;
    */
  
  //uint8_t primary_channel_AP_ftm;    // channel of AP for FTM purposes
  //uint8_t num_entries = 10;          // Number of entries you want to retrieve
  wifi_ftm_report_entry_t wifi_ftm_report[10]; // Adjust size based on expected entries
  std::string TAG_STA = "ftm_station";
  //uint32_t s_rtt_est, s_dist_est;

  //wifi_ftm_initiator_cfg_t ftm_initiator; // struct for esp_wifi_ftm_initiate_session(*ftm_initiator)
  //static uint32_t s_rtt_est, s_dist_est;


  void init_ftm(); //new finite time measurement (ftm) function for calculation of distance from router trialing
  //void ftm_monitor(void *prParameters); // function for xTaskCreate for FTM recurrent analysis
  void printBSSID(const uint8_t bssid[6]);
  void getFTMReport();
  void f2();

};
} 

/* event information from esp-idf referred version
	 * FTM
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_FTM_REPORT) {
    	arduino_event.event_id = "FTM_REPORT";
    	memcpy(&arduino_event.event_info.wifi_ftm_report, event_data, sizeof(wifi_event_ftm_report_t));
*/