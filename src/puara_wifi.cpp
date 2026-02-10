
#include "puara_config.hpp"
#include "puara_ftm.hpp"
#include "puara_logger.hpp"
#include "puara_wifi.hpp"

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
// Set device as FTM responder capable
  this->wifi_config_ap.ap.ftm_responder = true;
//
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
    ESP_LOGE(PUARA_TAG,"wifi_init: failed to set hostname: %s", config.dmiName.c_str());
  }
  else
  {
    ESP_LOGI(PUARA_TAG,"wifi_init: hostname: %s", config.dmiName.c_str());
  }

  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &WiFi::sta_event_handler, this, &instance_any_id));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &WiFi::sta_event_handler, this, &instance_got_ip));

  ESP_LOGI(PUARA_TAG,"wifi_init: setting wifi mode");
  if(config.persistentAP)
  {
    ESP_LOGI(PUARA_TAG,"wifi_init:     AP-STA mode");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_LOGI(PUARA_TAG,"wifi_init: loading AP config");
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &this->wifi_config_ap));
  }
  else
  {
    ESP_LOGI(PUARA_TAG,"wifi_init:     STA mode");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  }
  std::cout << "wifi_init: loading STA config" << std::endl;
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &this->wifi_config_sta));
  std::cout << "wifi_init: esp_wifi_start" << std::endl;
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(PUARA_TAG,"wifi_init: wifi_init finished.");

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
    ESP_LOGI(PUARA_TAG,"wifi_init: Connected to SSID: %s", config.wifiSSID.c_str());
    currentSSID = config.wifiSSID;
    this->StaIsConnected = true;
  }
  else if(bits & this->wifi_fail_bit)
  {
    ESP_LOGW(PUARA_TAG,"wifi_init: Failed to connect to SSID: %s", config.wifiSSID.c_str());
    if(!config.persistentAP)
    {
      ESP_LOGW(PUARA_TAG,"wifi_init: Failed to connect to SSID: %s", config.wifiSSID.c_str());
      ESP_LOGI(PUARA_TAG,"Switching to AP/STA mode");
      ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
      ESP_LOGI(PUARA_TAG,"wifi_init: loading AP config");
      ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &this->wifi_config_ap));
      ESP_LOGW(PUARA_TAG,"wifi_init: Trying to connect one more time to SSID before giving up.");
      ESP_ERROR_CHECK(esp_wifi_start());
    }
    else
    {
      this->StaIsConnected = false;
    }
  }
  else
  {
    ESP_LOGE(PUARA_TAG,"wifi_init: UNEXPECTED EVENT");
  }

  // Unregister IP and WIFI event handlers
  /*
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
      IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
  */
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
      WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
  

  esp_event_handler_instance_t instance_got_ftm_report;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, WIFI_EVENT_FTM_REPORT, &WiFi::sta_event_handler, this, &instance_got_ftm_report));

  // Small delay to ensure any in-flight event handlers complete before deleting the event group
  vTaskDelay(pdMS_TO_TICKS(100));
  
  vEventGroupDelete(this->s_wifi_event_group);
  this->s_wifi_event_group = NULL;

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
  ESP_LOGI(PUARA_TAG,"wifi_scan: Total APs scanned = %d", ap_count);
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
    ESP_LOGW(PUARA_TAG,"start_wifi: Module name unpopulated. Using default name: Puara");
    config.dmiName = "Puara";
  }
  if(config.APpasswd.empty() || config.APpasswd.length() < 8
     || config.APpasswd == "password")
  {
    ESP_LOGW(PUARA_TAG,"startWifi: AP password error. Possible causes:");
    ESP_LOGW(PUARA_TAG,"startWifi:   - no AP password");
    ESP_LOGW(PUARA_TAG,"startWifi:   - password is less than 8 characters long");
    ESP_LOGW(PUARA_TAG,"startWifi:   - password is set to \"password\"");
    ESP_LOGW(PUARA_TAG,"startWifi: Using default AP password: password");
    ESP_LOGW(PUARA_TAG,"startWifi: It is strongly recommended to change the password");
    config.APpasswd = "password";
  }
  if(config.wifiSSID.empty())
  {
    ESP_LOGW(PUARA_TAG,"start_wifi: No blank SSID allowed. Using default name: Puara");
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

  ESP_LOGI(PUARA_TAG,"startWifi: Starting WiFi config");
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
    }
    else
    {
      if(self.s_wifi_event_group != NULL)
      {
        xEventGroupSetBits(self.s_wifi_event_group, self.wifi_fail_bit);
      }
      ESP_LOGW(PUARA_TAG, "Connect to external AP failed. Creating own Access Point network");
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
    ESP_LOGI(PUARA_TAG, "Connected to external AP with IP %s", self.currentSTA_IP.c_str());
    self.connect_counter = 0;
    if(self.s_wifi_event_group != NULL)
    {
      xEventGroupSetBits(self.s_wifi_event_group, self.wifi_connected_bit);
    }
  
    // FTM implementation
    wifi_ap_record_t ap_info;
    if(esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK){
      if(self.ftm){
        std::copy(std::begin(ap_info.bssid), std::end(ap_info.bssid), std::begin(self.ftm->externalAP_BSSID));
        self.ftm->ftm_channel = ap_info.primary;
        self.ftm->ftm_responder_state = ap_info.ftm_responder;
        //self.ftm_initiator_state = ap_info.ftm_initiator; to know if responder is also initiator is not necessary right now

      ESP_LOGD(PUARA_TAG, "Channel of external AP: %d", (int)ap_info.primary );
      ESP_LOGD(PUARA_TAG, "Does external AP support FTM Responder Mode (0-no/1-yes): %d", (int)ap_info.ftm_responder );  
      ESP_LOGD(PUARA_TAG, "Is external AP also FTM Initiator (0-no/1-yes): %d", (int)ap_info.ftm_initiator );


      }  // end if(self.ftm)
    }  // end if(esp_wifi_sta_get_ap_info)
    //end FTM implementation tests to get active Router MAC address

  }
  else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_FTM_REPORT){
    wifi_event_ftm_report_t* report = (wifi_event_ftm_report_t*) event_data;

    if(report->status == FTM_STATUS_SUCCESS){
      ESP_LOGV(PUARA_TAG, "FTM Report: RTT: %u ns, Distance: %u cm", report->rtt_est, report->dist_est);
      self.ftm->last_rtt_ns = report->rtt_est;
      self.ftm->last_distance_cm = report->dist_est;
      self.ftm->ftm_report_available = true;
    }else{
      ESP_LOGW(PUARA_TAG, "FTM Report received with non-success status: %d", report->status);
    }
    // ftm_report_num_entries x sizeof(wifi_ftm_report_entry_t) allocated on heap every cycle to be freed by caller.
    if (report->ftm_report_data != NULL){
      free(report->ftm_report_data);
    }
  
  }
  else {
    // Debug: print unhandled events
    ESP_LOGD(PUARA_TAG, "Unhandled WiFi event - base: %s, id: %ld", event_base, event_id);
  }
}

bool WiFi::get_StaIsConnected()
{
  return StaIsConnected;
}
}
