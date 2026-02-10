#include "puara_ftm.hpp"
#include "puara_logger.hpp"
#include "puara_wifi.hpp"

// #include <.h>
//#include <iostream>

namespace PuaraAPI
{

void FTM::configureFTM(uint8_t frm_cnt, uint16_t burst_prd){

    ESP_LOGD(PUARA_TAG, "Configuring FTM parameters");
    //get responder MAC address
    std::copy(std::begin(this->currentRouter_BSSID), std::end(this->currentRouter_BSSID), std::begin(wifi_ftm_cfg.resp_mac));
    wifi_ftm_cfg.channel = this->ftm_channel;
    wifi_ftm_cfg.frm_count = frm_cnt; // No. of FTM frames requested in terms of 4 or 8 bursts (allowed values - 0(No pref), 16, 24, 32, 64)    
    wifi_ftm_cfg.burst_period = burst_prd; // Requested time period between consecutive FTM bursts in 100's of milliseconds (0 - ASAP)
}

void FTM::requestFTM(){
    esp_err_t result = esp_wifi_ftm_initiate_session(&wifi_ftm_cfg);
    if (result != ESP_OK) {
        ESP_LOGD(PUARA_TAG, "FTM Session Failed : %s", esp_err_to_name(result));
    }else if(result == ESP_OK) {
        ESP_LOGD(PUARA_TAG, "FTM session initiated successfully");
    }
}

}