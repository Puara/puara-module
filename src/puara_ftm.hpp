#pragma once

#include "esp_log.h"
#include <esp_wifi.h>

#include <iomanip>
#include <iostream>
#include <string>

/*
Supported Targets 	ESP32-C2 	ESP32-C3 	ESP32-C5 	ESP32-C6 	ESP32-C61 	ESP32-S2 	ESP32-S3
*/

namespace PuaraAPI
{

struct WiFi; // forward declaration

struct FTM
{
    WiFi& wifi;
    int32_t last_rtt_ns = 0;
    int32_t last_distance_cm = 0;


    wifi_ftm_initiator_cfg_t wifi_ftm_cfg = {};
  /*  FTM Initiator configuration                 confirm these values with esp-idf docs 5.5.2
    typedef struct {
      uint8_t resp_mac[6];        // MAC address of the FTM Responder 
      uint8_t channel;            // Primary channel of the FTM Responder 
      uint8_t frm_count;          // No. of FTM frames requested in terms of 4 or 8 bursts (allowed values - 0(No pref), 16, 24, 32, 64) 
      uint16_t burst_period;      // Requested time period between consecutive FTM bursts in 100's of milliseconds (0 - No pref) 
} wifi_ftm_initiator_cfg_t;
*/
    wifi_ftm_status_t wifi_ftm_status;
  /*typedef enum {                      confirm these values with esp-idf docs 5.5.2
    FTM_STATUS_SUCCESS = 0,     // FTM exchange is successful 
    FTM_STATUS_UNSUPPORTED,     // Peer does not support FTM 
    FTM_STATUS_CONF_REJECTED,   // Peer rejected FTM configuration in FTM Request 
    FTM_STATUS_NO_RESPONSE,     // Peer did not respond to FTM Requests 
    FTM_STATUS_FAIL,            // Unknown error during FTM exchange 
} wifi_ftm_status_t;
*/
    wifi_ftm_report_entry_t wifi_ftm_report_entry;
/** Argument structure for wifi_ftm_report_entry_t          confirm these values with esp-idf docs 5.5.2
Adjust size based on expected entries
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

    void configureFTM(); //new finite time measurement (ftm) function for calculation of distance from router trialing
    void requestFTM(); // function to trigger FTM procedure and get report in event handler

};
} 
/*    USED IN puara_wifi.cpp line 273  for FTM
struct wifi_ap_record_t
    Description of a Wi-Fi AP.

    Public Members

**  uint8_t bssid[6] : MAC address of AP
    uint8_t ssid[33] : SSID of AP
**  uint8_t primary :  Channel of AP
    wifi_second_chan_t second : Secondary channel of AP
**  int8_t rssi : Signal strength of AP. Note that in some rare cases where signal strength is very strong, RSSI values can be slightly positive
    wifi_auth_mode_t authmode : Auth mode of AP
    wifi_cipher_type_t pairwise_cipher : Pairwise cipher of AP
    wifi_cipher_type_t group_cipher :  Group cipher of AP
    wifi_ant_t ant : Antenna used to receive beacon from AP
    uint32_t phy_11b :  Bit: 0 flag to identify if 11b mode is enabled or not
    uint32_t phy_11g :  Bit: 1 flag to identify if 11g mode is enabled or not
    uint32_t phy_11n :  Bit: 2 flag to identify if 11n mode is enabled or not
    uint32_t phy_lr :   Bit: 3 flag to identify if low rate is enabled or not
    uint32_t phy_11a :  Bit: 4 flag to identify if 11ax mode is enabled or not
    uint32_t phy_11ac : Bit: 5 flag to identify if 11ax mode is enabled or not
    uint32_t phy_11ax : Bit: 6 flag to identify if 11ax mode is enabled or not
    uint32_t wps :      Bit: 7 flag to identify if WPS is supported or not

**  uint32_t ftm_responder : Bit: 8 flag to identify if FTM is supported in responder mode
**  uint32_t ftm_initiator :  Bit: 9 flag to identify if FTM is supported in initiator mode

    uint32_t reserved : Bit: 10..31 reserved
    wifi_country_t country :  Country information of AP
    wifi_he_ap_info_t he_ap : HE AP info
    wifi_bandwidth_t bandwidth : Bandwidth of AP
    uint8_t vht_ch_freq1 : This fields are used only AP bandwidth is 80 and 160 MHz, to transmit the center channel frequency of the BSS. For AP bandwidth is 80 + 80 MHz, it is the center channel frequency of the lower frequency segment.
    uint8_t vht_ch_freq2 :  this fields are used only AP bandwidth is 80 + 80 MHz, and is used to transmit the center channel frequency of the second segment.
*/
