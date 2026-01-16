#pragma once
#define CONFIG_LOG_MASTER_LEVEL ESP_LOG_VERBOSE

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

namespace PuaraAPI
{
static const char* PUARA_TAG = "puara-module";
}
