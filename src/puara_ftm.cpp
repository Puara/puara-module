#include "puara_ftm.hpp"
#include "puara_logger.hpp"
#include "puara_wifi.hpp"

// #include <.h>
//#include <iostream>

namespace PuaraAPI
{

void FTM::initiateFTM(){

    //Start an FTM Initiator session by sending FTM request
    //If successful, event WIFI_EVENT_FTM_REPORT is generated with the result of the FTM procedure. 
    //Attention :   
    //    1. Use this API only in Station mode
    //    2. If FTM is initiated on a different channel than Station is connected in or internal
    //       SoftAP is started in, FTM defaults to a single burst in ASAP mode

    std::cout << "Initialising FTM" << std::endl;

    //get responder MAC address
    std::copy(std::begin(wifi.currentRouter_BSSID), std::end(wifi.currentRouter_BSSID), std::begin(wifi_ftm_cfg.resp_mac));

    wifi_ftm_cfg.channel = wifi.ftm_channel; // = primary_channel_AP_ftm;
    wifi_ftm_cfg.frm_count = 16; // No. of FTM frames requested in terms of 4 or 8 bursts (allowed values - 0(No pref), 16, 24, 32, 64)    
    wifi_ftm_cfg.burst_period = 2; // Requested time period between consecutive FTM bursts in 100's of milliseconds (0 - No pref)

    esp_err_t result = esp_wifi_ftm_initiate_session(&wifi_ftm_cfg);
    if (result != ESP_OK) {
        std::cout << "FTM initiate session failed :: result (esp_err_t) : " << result << "\n" << std::endl;
    }else if(result == ESP_OK) {
        std::cout << "FTM initiate session OK :: result (esp_err_t) : " << result << "\n" << std::endl;
    }
}

void FTM::getFTMReport(){
/*
    esp_err_t result = esp_wifi_ftm_get_report(report, num_entries);
        if (result == ESP_OK) {
        // Process the report data
        std::cout << "FTM :: RoundTrip Time : " << report[0].rtt << std::endl;
        std::cout << "FTM :: RSSI : " << report[0].rssi << std::endl;
        } else {
        // Handle error
        std::cout << "Error retrieving FTM report: " << result << std::endl;
       } */ 
}


  //void ftm_monitor(void *prParameters); // function for xTaskCreate for FTM recurrent analysis





}