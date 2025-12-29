#pragma once

#if defined(PUARA_SPIFFS)
#include "puara_spiffs.hpp"
#else
#include "puara_littlefs.hpp"
#endif

#include <esp_err.h>

#include <unordered_map>
#include <vector>
#include <string>


namespace PuaraAPI
{
struct DeviceConfiguration;
struct Serial;
struct Webserver;

// FIXME std::variant<std::string, double> instead
struct settingsVariables
{
  std::string name;
  std::string type;
  std::string textValue;
  double numberValue;
};
struct JSONSettings // TODO: remove from puara_filesystem
{
  DeviceConfiguration& config;
  PuaraFileSystem& fs;

  // public API:
  void read_config_json();
  void write_config_json();
  void read_settings_json();
  void write_settings_json();

  void set_settings_changed_handler(std::function<void()>);

  double getVarNumber(std::string varName);
  std::string getVarText(std::string varName);

  friend class Webserver;
  friend class Serial;

  // Private API used by puara_web
  std::vector<settingsVariables> variables;
  std::unordered_map<std::string, int> variables_fields;
  std::function<void()> on_settings_changed;

  // Private API used by puara_serial
  void read_settings_json_internal(std::string& contents, bool merge = false);
  void read_config_json_internal(std::string& contents);
  void merge_settings_json(std::string& new_contents);
};
}
