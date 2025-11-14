#pragma once
#include <esp_err.h>
#include <esp_spi_flash.h>
#include <esp_spiffs.h>
#include <LittleFS.h>

#include <unordered_map>
#include <vector>
#include <string>

namespace PuaraAPI
{
struct DeviceConfiguration;
struct Serial;
struct Webserver;

class PuaraFileSystem {
 public:
  virtual void mount() = 0;
  virtual void unmount() = 0;
  virtual std::string read_file(const std::string& path) = 0;
  virtual void write_file(const std::string& path, const std::string& content) = 0;
};

class SPIFFS: public PuaraFileSystem
{
public:
  void mount() override;
  void unmount() override;
  std::string read_file(const std::string& path) override;
  void write_file(const std::string& path, const std::string& content) override;

private:
  esp_vfs_spiffs_conf_t spiffs_config;
  std::string spiffs_base_path = "/spiffs/";
};

struct LITTLEFS: public PuaraFileSystem
{
public:
  void mount() override;
  void unmount() override;
  std::string read_file(const std::string& path) override;
  void write_file(const std::string& path, const std::string& content) override;
};

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
  PuaraFileSystem* fs;

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
