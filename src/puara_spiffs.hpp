#pragma once
#include <esp_err.h>
#include <esp_spi_flash.h>
#include <esp_spiffs.h>

#include <unordered_map>
#include <vector>
#include <string>

namespace PuaraAPI
{
struct DeviceConfiguration;
struct Serial;
struct Webserver;

struct SPIFFS
{
public:
  void config_spiffs();
  void mount_spiffs();
  void unmount_spiffs();

private:
  esp_vfs_spiffs_conf_t spiffs_config;
  std::string spiffs_base_path;
};

// FIXME std::variant<std::string, double> instead
struct settingsVariables
{
  std::string name;
  std::string type;
  std::string textValue;
  double numberValue;
};

struct JSONSettings
{
  DeviceConfiguration& config;
  SPIFFS& spiffs;

  // public API:
  void read_config_json();
  void write_config_json();
  void read_settings_json();
  void write_settings_json();

  double getVarNumber(std::string varName);
  std::string getVarText(std::string varName);

  friend class Webserver;
  friend class Serial;
  // Private API used by puara_web
  std::vector<settingsVariables> variables;
  std::unordered_map<std::string, int> variables_fields;

  // Private API used by puara_serial
  void read_settings_json_internal(std::string& contents, bool merge = false);
  void read_config_json_internal(std::string& contents);
  void merge_settings_json(std::string& new_contents);
};
} 