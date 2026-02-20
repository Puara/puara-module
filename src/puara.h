//****************************************************************************//
// Puara Module Manager - WiFi and file system functions                      //
// Metalab - Société des Arts Technologiques (SAT)                            //
// Input Devices and Music Interaction Laboratory (IDMIL), McGill University  //
// Edu Meneses (2022) - https://www.edumeneses.com                            //
//****************************************************************************//

#ifndef PUARA_H
#define PUARA_H


#include "puara_config.hpp"
#include "puara_ftm.hpp"

#include <string>
#include <string_view>
#include <functional>

typedef void* httpd_handle_t;
class Puara
{
public:
  void start(PuaraAPI::Monitors monitor = PuaraAPI::UART_MONITOR, esp_log_level_t debug_level=ESP_LOG_WARN);

  httpd_handle_t start_webserver(void);
  void stop_webserver(void);

  std::string dmi_name();
  unsigned int version();
  void set_version(unsigned int user_version);

  void mount();
  void unmount();

  void read_config_json();
  void write_config_json();
  void read_settings_json();
  void write_settings_json();

  void set_settings_changed_handler(std::function<void()>);

  bool start_serial_listening();
  void send_serial_data(std::string data);

  void start_mdns_service(std::string_view device_name, std::string_view instance_name);

  void start_wifi();
  void wifi_scan();
  bool get_StaIsConnected();

   // returns current IP address on external wifi access point
  std::string staIP();

// FTM related functions
  int get_num_responder_aps();
  bool ftm_report_available();
  const std::map<std::string, wifi_ftm_initiator_cfg_t> get_map_of_responder_configs(
    uint8_t frame_count, uint16_t burst_period, const std::vector<std::string>& returned_ssids); 
  void set_ftm_report_as_consumed();
        
  
  double getVarNumber(std::string varName);
  std::string getVarText(std::string varName);
};

#endif
