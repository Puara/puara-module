#include "puara_wifi.hpp"

#include "puara_config.hpp"

#include <nvs_flash.h>

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>

namespace PuaraAPI
{

static constexpr short int channel = 6;
static constexpr short int max_connection = 5;
static constexpr short int wifi_maximum_retry = 5;
static constexpr int wifiScanSize = 20;

void WiFi::wifi_init()
{
  this->s_wifi_event_group = xEventGroupCreate();

  ESP_ERROR_CHECK(esp_netif_init());

  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();
  esp_netif_t* ap_netif = esp_netif_create_default_wifi_ap(); // saving pointer to
                                                              // retrieve AP ip later

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  // Set device hostname
  esp_err_t setname = esp_netif_set_hostname(ap_netif, config.dmiName.c_str());
  if(setname != ESP_OK)
  {
    LOG("wifi_init: failed to set hostname: ");
    LOG(config.dmiName.c_str());
  }
  else
  {
    LOG("wifi_init: hostname: ");
    LOG(config.dmiName);
  }

  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &WiFi::sta_event_handler, this, &instance_any_id));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &WiFi::sta_event_handler, this, &instance_got_ip));

  LOG("wifi_init: setting wifi mode");
  if(config.persistentAP)
  {
    LOG("wifi_init:     AP-STA mode");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    LOG("wifi_init: loading AP config");
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &this->wifi_config_ap));
  }
  else
  {
    LOG("wifi_init:     STA mode");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  }
  LOG("wifi_init: loading STA config");
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &this->wifi_config_sta));
  LOG("wifi_init: esp_wifi_start");
  ESP_ERROR_CHECK(esp_wifi_start());

  LOG("wifi_init: wifi_init finished.");

  /* Waiting until either the connection is established (this->wifi_connected_bit)
   * or connection failed for the maximum number of re-tries (this->wifi_fail_bit).
   * The bits are set by event_handler() (see above) */
  EventBits_t bits = xEventGroupWaitBits(
      this->s_wifi_event_group, this->wifi_connected_bit | this->wifi_fail_bit, pdFALSE,
      pdFALSE, portMAX_DELAY);

  /* xEventGroupWaitBits() returns the bits before the call returned, hence we
   * can test which event actually happened. */
  if(bits & this->wifi_connected_bit)
  {
    LOG("wifi_init: Connected to SSID: ");
    LOG(config.wifiSSID);
    currentSSID = config.wifiSSID;
    this->StaIsConnected = true;
  }
  else if(bits & this->wifi_fail_bit)
  {
    LOG("wifi_init: Failed to connect to SSID: ");
    LOG(config.wifiSSID);
    if(!config.persistentAP)
    {
      LOG("wifi_init: Failed to connect to SSID: ");
      LOG(config.wifiSSID);
      LOG("Switching to AP/STA mode");
      ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
      LOG("wifi_init: loading AP config");
      ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &this->wifi_config_ap));
      LOG("wifi_init: Trying to connect one more time to SSID before giving up.");
      ESP_ERROR_CHECK(esp_wifi_start());
    }
    else
    {
      this->StaIsConnected = false;
    }
  }
  else
  {
    LOG("wifi_init: UNEXPECTED EVENT");
  }

  /* The event will not be processed after unregister */
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
      IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
      WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
  vEventGroupDelete(this->s_wifi_event_group);

  // getting extra info
  unsigned char temp_info[6] = {0};
  esp_wifi_get_mac(WIFI_IF_STA, temp_info);
  std::ostringstream tempBuf;
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[0] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[1] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[2] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[3] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[4] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[5];
  this->currentSTA_MAC = tempBuf.str();
  tempBuf.clear();            // preparing the ostringstream
  tempBuf.str(std::string()); // buffer for reuse
  esp_wifi_get_mac(WIFI_IF_AP, temp_info);
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[0] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[1] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[2] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[3] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[4] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[5];
  this->currentAP_MAC = tempBuf.str();

  esp_netif_ip_info_t ip_temp_info;
  esp_netif_get_ip_info(ap_netif, &ip_temp_info);
  tempBuf.clear();
  tempBuf.str(std::string());
  tempBuf << std::dec << esp_ip4_addr1_16(&ip_temp_info.ip) << ".";
  tempBuf << std::dec << esp_ip4_addr2_16(&ip_temp_info.ip) << ".";
  tempBuf << std::dec << esp_ip4_addr3_16(&ip_temp_info.ip) << ".";
  tempBuf << std::dec << esp_ip4_addr4_16(&ip_temp_info.ip);
  this->currentAP_IP = tempBuf.str();
}

void WiFi::wifi_scan(void)
{
  uint16_t number = PuaraAPI::wifiScanSize;
  wifi_ap_record_t ap_info[PuaraAPI::wifiScanSize];
  uint16_t ap_count = 0;
  memset(ap_info, 0, sizeof(ap_info));

  esp_wifi_scan_start(NULL, true);
  ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
  ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
  LOG("wifi_scan: Total APs scanned = ");
  LOG(std::to_string(ap_count).c_str());
  wifiAvailableSsid.clear();
  for(int i = 0; (i < PuaraAPI::wifiScanSize) && (i < ap_count); i++)
  {
    wifiAvailableSsid.append("<strong>SSID: </strong>");
    wifiAvailableSsid.append(reinterpret_cast<const char*>(ap_info[i].ssid));
    wifiAvailableSsid.append("<br>      (RSSI: ");
    wifiAvailableSsid.append(std::to_string(ap_info[i].rssi));
    wifiAvailableSsid.append(", Channel: ");
    wifiAvailableSsid.append(std::to_string(ap_info[i].primary));
    wifiAvailableSsid.append(")<br>");
  }
}

void WiFi::start_wifi()
{
  this->ApStarted = false;

  // Check if wifiSSID is empty and wifiPSK have less than 8 characteres
  if(config.dmiName.empty())
  {
    LOG("start_wifi: Module name unpopulated. Using default name: Puara");
    config.dmiName = "Puara";
  }
  if(config.APpasswd.empty() || config.APpasswd.length() < 8
     || config.APpasswd == "password")
  {
    LOG("startWifi: AP password error. Possible causes:");
    LOG("startWifi:   - no AP password");
    LOG("startWifi:   - password is less than 8 characteres long");
    LOG("startWifi:   - password is set to \"password\"");
    LOG("startWifi: Using default AP password: password");
    LOG("startWifi: It is strongly recommended to change the password");
    config.APpasswd = "password";
  }
  if(config.wifiSSID.empty())
  {
    LOG("start_wifi: No blank SSID allowed. Using default name: Puara");
    config.wifiSSID = "Puara";
  }

  strncpy(
      (char*)this->wifi_config_sta.sta.ssid, config.wifiSSID.c_str(),
      config.wifiSSID.length() + 1);
  strncpy(
      (char*)this->wifi_config_sta.sta.password, config.wifiPSK.c_str(),
      config.wifiPSK.length() + 1);
  strncpy(
      (char*)this->wifi_config_ap.ap.ssid, config.dmiName.c_str(),
      config.dmiName.length() + 1);
  this->wifi_config_ap.ap.ssid_len = config.dmiName.length();
  this->wifi_config_ap.ap.channel = PuaraAPI::channel;
  strncpy(
      (char*)this->wifi_config_ap.ap.password, config.APpasswd.c_str(),
      config.APpasswd.length() + 1);
  this->wifi_config_ap.ap.max_connection = PuaraAPI::max_connection;
  this->wifi_config_ap.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  LOG("startWifi: Starting WiFi config");
  this->connect_counter = 0;
  wifi_init();
  this->ApStarted = true;
}

void WiFi::sta_event_handler(
    void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
  WiFi& self = *static_cast<WiFi*>(arg);
  // int counter = 0;
  if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
  {
    esp_wifi_connect();
  }
  else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
  {
    std::cout << "Trying to connect to AP " << self.connect_counter << "/" << PuaraAPI::wifi_maximum_retry << std::endl;
    if(self.connect_counter < PuaraAPI::wifi_maximum_retry)
    {
      self.connect_counter++;
      esp_wifi_connect();
      LOG("wifi/sta_event_handler: retry to connect to the AP");
    }
    else
    {
      xEventGroupSetBits(self.s_wifi_event_group, self.wifi_fail_bit);
      std::cout << "Connect to external AP failed. Creating own Access Point network\n" << std::endl;
    }
  }
  else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
  {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;

    std::stringstream tempBuf;
    tempBuf << esp_ip4_addr1_16(&event->ip_info.ip) << ".";
    tempBuf << esp_ip4_addr2_16(&event->ip_info.ip) << ".";
    tempBuf << esp_ip4_addr3_16(&event->ip_info.ip) << ".";
    tempBuf << esp_ip4_addr4_16(&event->ip_info.ip);
    self.currentSTA_IP = tempBuf.str();
    LOG("wifi/sta_event_handler: got ip:");
    LOG(self.currentSTA_IP);
    self.connect_counter = 0;
    xEventGroupSetBits(self.s_wifi_event_group, self.wifi_connected_bit);
  }
}

bool WiFi::get_StaIsConnected()
{
  return StaIsConnected;
}
} 
