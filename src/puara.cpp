/*

Puara Module Manager
Metalab - Société des Arts Technologiques (SAT)
Input Devices and Music Interaction Laboratory (IDMIL), McGill University
Edu Meneses (2022) - https://www.edumeneses.com

- event_handler, wifi_init_sta, and start_wifi were modified from
  https://github.com/espressif/esp-idf/tree/master/examples/wifi/getting_started/station
- mount_spiffs, and unmount_spiffs were modified from
  https://github.com/espressif/esp-idf/tree/master/examples/storage

*/

#include "puara_config.hpp"
#include "puara_device.hpp"
#include "puara_logger.hpp"
#include "puara_filesystem.hpp"
#include "puara_mdns.hpp"
#include "puara_serial.hpp"
#include "puara_web.hpp"
#include "puara_wifi.hpp"
#include <memory>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <puara.h>

#include <iostream>
#include <optional>

struct PuaraGlobal
{
  PuaraAPI::DeviceConfiguration config;
  PuaraAPI::Device device;
  PuaraAPI::PuaraFileSystem fs;
  PuaraAPI::JSONSettings settings{config, fs};
  PuaraAPI::Serial serial{config, device, fs, settings};
  PuaraAPI::WiFi wifi{config};
  PuaraAPI::Webserver webserver{config, device, fs, settings, wifi};
  PuaraAPI::MDNSService mdns;

  PuaraGlobal() { }

  void start(PuaraAPI::Monitors monitor, esp_log_level_t debug_level)
  {
    // Defaults to ESP_LOG_WARNING so that only warnings and errors are printed.
    esp_log_level_set("puara-module", debug_level);
    std::cout << "\n"
              << "Puara Module Manager                                   \n"
              << "Innovation - Société des Arts Technologiques (SAT)     \n"
              << "Input Devices and Music Interaction Laboratory (IDMIL) \n"
              << "Firmware version: " << config.version << "\n"
              << std::endl;

    settings.read_config_json();
    settings.read_settings_json();
    wifi.start_wifi();
    webserver.start_webserver();
    mdns.start(config.dmiName, config.dmiName);
    wifi.wifi_scan();

    serial.module_monitor = monitor;

    // some delay added as start listening blocks the hw monitor
    ESP_LOGI(PuaraAPI::PUARA_TAG,"Starting serial monitor...");
    vTaskDelay(50 / portTICK_RATE_MS);
    if(serial.start_serial_listening()){ // ??
      };
    vTaskDelay(50 / portTICK_RATE_MS);
    ESP_LOGI(PuaraAPI::PUARA_TAG,"serial listening ready");

    std::cout << "Puara Start Done!\n\n  Type \"reboot\" in the serial monitor to reset "
                 "the ESP32.\n\n";
  }

};

static PuaraGlobal g_puara;

// Defining static members
void Puara::start(PuaraAPI::Monitors monitor, esp_log_level_t debug_level)
{
  g_puara.start(monitor, debug_level);
}

httpd_handle_t Puara::start_webserver(void)
{
  return g_puara.webserver.start_webserver();
}
void Puara::stop_webserver(void)
{
  return g_puara.webserver.stop_webserver();
}

std::string Puara::dmi_name()
{
  return g_puara.config.dmiName;
}
unsigned int Puara::version()
{
  return g_puara.config.version;
}
void Puara::set_version(unsigned int user_version)
{
  g_puara.config.version = user_version;
}

void Puara::mount()
{
  return g_puara.fs.mount();
}
void Puara::unmount()
{
  return g_puara.fs.unmount();
}

void Puara::read_config_json()
{
  return g_puara.settings.read_config_json();
}
void Puara::write_config_json()
{
  return g_puara.settings.write_config_json();
}
void Puara::read_settings_json()
{
  return g_puara.settings.read_settings_json();
}
void Puara::write_settings_json()
{
  return g_puara.settings.write_settings_json();
}

void Puara::set_settings_changed_handler(std::function<void()> func)
{
  return g_puara.settings.set_settings_changed_handler(std::move(func));
}

double Puara::getVarNumber(std::string varName)
{
  return g_puara.settings.getVarNumber(varName);
}
std::string Puara::getVarText(std::string varName)
{
  return g_puara.settings.getVarText(varName);
}

bool Puara::start_serial_listening()
{
  return g_puara.serial.start_serial_listening();
}
void Puara::send_serial_data(std::string data)
{
  return g_puara.serial.send_serial_data(data);
}

void Puara::start_mdns_service(
    std::string_view device_name, std::string_view instance_name)
{
  return g_puara.mdns.start(device_name, instance_name);
}

void Puara::start_wifi()
{
  return g_puara.wifi.start_wifi();
}
void Puara::wifi_scan(void)
{
  return g_puara.wifi.wifi_scan();
}
bool Puara::get_StaIsConnected()
{
  return g_puara.wifi.get_StaIsConnected();
}
