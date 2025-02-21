#include "puara_ftm.hpp"

namespace PuaraAPI
{
void puara_ftm::init_ftm(){

    //Start an FTM Initiator session by sending FTM request
    //If successful, event WIFI_EVENT_FTM_REPORT is generated with the result of the FTM procedure. 
    //Attention :   
    //    1. Use this API only in Station mode
    //    2. If FTM is initiated on a different channel than Station is connected in or internal
    //       SoftAP is started in, FTM defaults to a single burst in ASAP mode

    std::cout << "Initialising FTM" << "\n" << std::endl;
    // struct for esp_wifi_ftm_initiate_session(*ftm_initiator)
    //get responder MAC address
    ftmi_cfg.resp_mac = currentRouter_bssid; //uint8_t currentRouter_bssid[6];
    ftmi_cfg.channel = primary_channel_AP_ftm;
    ftmi_cfg.frm_count = 0; //0 means no preference    
    ftmi_cfg.burst_period = 0; // 0 means no preference
    //ftmi_cfg.use_get_report_api=false; /**< True - Using esp_wifi_ftm_get_report to get FTM report, False - Using ftm_report_data from
    //                                              WIFI_EVENT_FTM_REPORT to get FTM report */

    esp_err_t result = esp_wifi_ftm_initiate_session(&ftmi_cfg);
    if (result != ESP_OK) {
        ESP_LOGE(TAG_STA, "Failed to start FTM session: %s", esp_err_to_name(result));
    }else if(result == ESP_OK) {
        std::cout << "FTM initiate session OK :: result :: " << result << "\n" << std::endl;
    }
}

void puara_ftm::printBSSID(const uint8_t bssid[6]){
    std::cout << "BSSID: ";
    for (int j = 0; j < 6; ++j) {
      currentRouter_bssid[j] = bssid[j];
      std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bssid[j]);
      if (j < 5) {
        std::cout << ":";
      }
    }
    std::cout << std::dec << std::endl;  // Switch back to decimal output
}

void puara_ftm::getFTMReport(){

    esp_err_t result = esp_wifi_ftm_get_report(report, num_entries);
        if (result == ESP_OK) {
        // Process the report data
        std::cout << "FTM :: RoundTrip Time : " << report[0].rtt << std::endl;
        std::cout << "FTM :: RSSI : " << report[0].rssi << std::endl;
        } else {
        // Handle error
        std::cout << "Error retrieving FTM report: " << result << std::endl;
       } 
}


void ftm_data::f2()
{
    return;
}

  //void ftm_monitor(void *prParameters); // function for xTaskCreate for FTM recurrent analysis
} 