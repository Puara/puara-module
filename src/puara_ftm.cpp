#include "puara_ftm.hpp"
#include "puara_logger.hpp"
#include "puara_wifi.hpp"

#include <unordered_set>

// #include <.h>
//#include <iostream>

namespace PuaraAPI
{

  // Return a map of responders defined by user and their wifi_ftm_cfg_t struct info (MAC, channel)
  const std::map<std::string, wifi_ftm_initiator_cfg_t> FTM::get_map_of_responder_configs(
    uint8_t frame_count, uint16_t burst_period, 
    const std::vector<std::string>& returned_ssids)
  {

    std::map<std::string, wifi_ftm_initiator_cfg_t> responder_configs;
    std::unordered_set<std::string> wanted_ssids(returned_ssids.begin(), returned_ssids.end());

    for (const auto& ap : scanned_responder_aps) {
      // Check if the AP is in the list of returned_ssids from the web interface (i.e. user selected it for FTM)
      if (wanted_ssids.count(ap.ssid)) {
        wifi_ftm_initiator_cfg_t ftm_config{};
        std::copy(std::begin(ap.bssid), std::end(ap.bssid), std::begin(ftm_config.resp_mac));
        ftm_config.channel = ap.primary_channel;
        ftm_config.frm_count = frame_count;
        ftm_config.burst_period = burst_period;
        ftm_config.use_get_report_api = false; // use WIFI_EVENT_FTM_REPORT to get FTM report
        responder_configs[ap.ssid] = ftm_config;

        ESP_LOGI(PUARA_TAG, "Added FTM responder config for AP: SSID: %s, BSSID: %02x:%02x:%02x:%02x:%02x:%02x, Channel: %d", 
          ap.ssid.c_str(),  
          ap.bssid[0], ap.bssid[1], ap.bssid[2], ap.bssid[3], ap.bssid[4], ap.bssid[5],
          ap.primary_channel
        );
      }
    }
    return responder_configs;

  } 




}

/*
//make this take in a position from an array of wifi_ftm_cfg 
void FTM::requestFTM(){
    esp_err_t result = esp_wifi_ftm_initiate_session(&wifi_ftm_cfg);
    if (result != ESP_OK) {
        ESP_LOGD(PUARA_TAG, "FTM Session Failed : %s", esp_err_to_name(result));
    }else if(result == ESP_OK) {
        ESP_LOGD(PUARA_TAG, "FTM session initiated successfully");
    }
}

bool FTM::set_offset_responder(int16_t offset_cm){
    // Set offset in cm for FTM Responder : 
    // An equivalent offset is calculated in picoseconds and added in TOD of FTM 
    // Measurement frame (T1).
    esp_err_t result = esp_wifi_ftm_resp_set_offset(offset_cm);
    if(result == ESP_OK) {
        ESP_LOGD(PUARA_TAG, "FTM responder offset set successfully");
        return true;
    } else {
        ESP_LOGD(PUARA_TAG, "Failed to set FTM responder offset : %s", esp_err_to_name(result));
        return false;
    }
}

void FTM::end_ftm_request_session(){
    //End the ongoing FTM Initiator session. This API works only on FTM Initiator
    esp_err_t result = esp_wifi_ftm_end_session();
    if(result == ESP_OK) {
        ESP_LOGD(PUARA_TAG, "FTM session ended successfully");
    } else {
        ESP_LOGD(PUARA_TAG, "Failed to end FTM session : %s", esp_err_to_name(result));
    }
}
*/