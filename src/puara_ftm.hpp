#pragma once

#include "esp_log.h"
#include <esp_wifi.h>

#include <iomanip>
#include <iostream>
#include <string>

namespace PuaraAPI
{

struct WiFi;

struct FTM
{
    WiFi& wifi;

    wifi_ftm_initiator_cfg_t wifi_ftm_cfg{};

    void configureFTM(uint8_t frm_cnt = 32, uint16_t burst_prd = 4); // Configure FTM parameters
    // Trigger FTM procedure and get report in event handler
    void requestFTM(); 

};
} 
