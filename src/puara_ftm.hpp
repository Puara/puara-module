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

  wifi_ftm_initiator_cfg_t wifi_ftm_cfg;
  /*  FTM Initiator configuration
    typedef struct {
      uint8_t resp_mac[6];        // MAC address of the FTM Responder 
      uint8_t channel;            // Primary channel of the FTM Responder 
      uint8_t frm_count;          // No. of FTM frames requested in terms of 4 or 8 bursts (allowed values - 0(No pref), 16, 24, 32, 64) 
      uint16_t burst_period;      // Requested time period between consecutive FTM bursts in 100's of milliseconds (0 - No pref) 
} wifi_ftm_initiator_cfg_t;
*/

  wifi_ftm_status_t wifi_ftm_status;
  /*typedef enum {
    FTM_STATUS_SUCCESS = 0,     // FTM exchange is successful 
    FTM_STATUS_UNSUPPORTED,     // Peer does not support FTM 
    FTM_STATUS_CONF_REJECTED,   // Peer rejected FTM configuration in FTM Request 
    FTM_STATUS_NO_RESPONSE,     // Peer did not respond to FTM Requests 
    FTM_STATUS_FAIL,            // Unknown error during FTM exchange 
} wifi_ftm_status_t;
*/
  wifi_ftm_report_entry_t wifi_ftm_report_entry; // Adjust size based on expected entries
/** Argument structure for wifi_ftm_report_entry_t
 * 
typedef struct {
    uint8_t dlog_token;     // Dialog Token of the FTM frame 
    int8_t rssi;            // RSSI of the FTM frame received
    uint32_t rtt;           // Round Trip Time in pSec with a peer 
    uint64_t t1;            // Time of departure of FTM frame from FTM Responder in pSec 
    uint64_t t2;            // Time of arrival of FTM frame at FTM Initiator in pSec 
    uint64_t t3;            // Time of departure of ACK from FTM Initiator in pSec 
    uint64_t t4;            // Time of arrival of ACK at FTM Responder in pSec 
} wifi_ftm_report_entry_t;
*/
//wifi_event_ftm_report_t wifi_ftm_report;
/** Argument structure for WIFI_EVENT_FTM_REPORT event
typedef struct {
    uint8_t peer_mac[6];                        // MAC address of the FTM Peer 
    wifi_ftm_status_t status;                   // Status of the FTM operation 
    uint32_t rtt_raw;                           // Raw average Round-Trip-Time with peer in Nano-Seconds 
    uint32_t rtt_est;                           // Estimated Round-Trip-Time with peer in Nano-Seconds 
    uint32_t dist_est;                          // Estimated one-way distance in Centi-Meters 
    wifi_ftm_report_entry_t *ftm_report_data;   // Pointer to FTM Report with multiple entries, should be freed after use 
    uint8_t ftm_report_num_entries;             // Number of entries in the FTM Report data 
} wifi_event_ftm_report_t;
*/

  std::string TAG_STA = "ftm_station";
  //uint32_t s_rtt_est, s_dist_est;

  //wifi_ftm_initiator_cfg_t ftm_initiator; // struct for esp_wifi_ftm_initiate_session(*ftm_initiator)
  //static uint32_t s_rtt_est, s_dist_est;


  void initiateFTM(); //new finite time measurement (ftm) function for calculation of distance from router trialing
  //void ftm_monitor(void *prParameters); // function for xTaskCreate for FTM recurrent analysis
  void getFTMReport();

};
} 

/* event information from esp-idf referred version
	 * FTM
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_FTM_REPORT) {
    	arduino_event.event_id = "FTM_REPORT";
    	memcpy(&arduino_event.event_info.wifi_ftm_report, event_data, sizeof(wifi_event_ftm_report_t));
*/