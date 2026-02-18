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

  /**
   * returns current IP address on external wifi access point
   */
  std::string staIP();


// FTM related functions
  int get_num_responder_aps();

  const std::vector<PuaraAPI::FTM::scanned_responder_ap_info>& get_scanned_responder_aps() const;

//  void FTM::configureFTM(uint8_t frame_count, uint16_t burst_period, uint8_t* target_bssid, uint8_t target_channel){

  /**
   * Trigger an FTM procedure and get the report in the event handler. Call this only on devices acting as FTM initiators (STAs connected to a responder AP). Do NOT call this on devices acting as solely as FTM responders (APs).
   */

//  void requestFTM();

  // Get last distance estimate in cm from FTM report
  uint32_t get_last_distance_cm();
  
  // Get last RTT estimate in ns from FTM report
  uint32_t get_last_rtt_ns();

  // Verify if FTM report data is available
  bool is_ftm_report_available();

  // Mark FTM report as consumed after reading the data to prevent stale data usage
  void set_ftm_report_as_consumed();
/*
  // Set offset in cm for FTM Responder.
  bool set_offset_responder(int16_t offset_cm);

  // End the ongoing FTM Initiator session.
  void end_ftm_request_session();
*/
  // Get RSSI of last FTM frame from connected external AP
  int get_rssi_of_ftm_frame();

  double getVarNumber(std::string varName);
  std::string getVarText(std::string varName);
};

#endif
