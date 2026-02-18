#include "puara_ftm.hpp"
#include "puara_logger.hpp"
#include "puara_wifi.hpp"

// #include <.h>
//#include <iostream>

namespace PuaraAPI
{
  // take in scanned_responder_aps and create wifi_ftm_cfgs vector based on user-defined parameters for frame count and burst period (same for all APs for now but can be made more flexible in the future) - this is to be called in setup after wifi_scan and before requestFTM
  std::vector<wifi_ftm_initiator_cfg_t> FTM::make_FTM_Configuration_Vectors(
    const std::vector<PuaraAPI::FTM::scanned_responder_ap_info>& responders, 
    uint8_t frame_count, 
    uint16_t burst_period){
    std::vector<wifi_ftm_initiator_cfg_t> ftm_configs;
    for (const auto& ap : responders) {
        wifi_ftm_initiator_cfg_t ftm_config{};
        std::copy(std::begin(ap.bssid), std::end(ap.bssid), std::begin(ftm_config.resp_mac));
        ftm_config.channel = ap.primary_channel;
        ftm_config.frm_count = frame_count;
        ftm_config.burst_period = burst_period;
        ftm_config.use_get_report_api = false; // use WIFI_EVENT_FTM_REPORT to get FTM report
        ftm_configs.push_back(ftm_config);
    }
    return ftm_configs;
}
// make a function that defines the trilaterion/quadrilaterion/n-eration 
//    (returns what?)  makeArrayOfFTMConfigs( int arraySize, scanned_ap_info* scannedAPs) 
/*
void FTM::configureFTM(uint8_t frame_count, uint16_t burst_period, uint8_t* target_bssid, uint8_t target_channel){

    ESP_LOGD(PUARA_TAG, "Configuring FTM parameters");
    //get responder MAC address
    std::copy(target_bssid, target_bssid + 6, wifi_ftm_cfg.resp_mac);
    wifi_ftm_cfg.channel = target_channel;
    wifi_ftm_cfg.frm_count = frame_count; // No. of FTM frames requested in terms of 4 or 8 bursts (allowed values - 0(No pref), 16, 24, 32, 64)    
    wifi_ftm_cfg.burst_period = burst_period; // Requested time period between consecutive FTM bursts in 100's of milliseconds (0 - ASAP)
}
*/

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
}