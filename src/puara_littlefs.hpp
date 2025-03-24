#pragma once
#include "puara_config.hpp"

#include <LittleFS.h>

#include <unordered_map>
#include <vector>

namespace PuaraAPI
{
struct DeviceConfiguration;
struct Serial;
struct Webserver;

struct LITTLEFS
{
public:
  void config_littlefs();
  void mount_littlefs();
  void unmount_littlefs();
  std::string read_file(const char* path);

private:
};

struct LittleFSJSONSettings // TODO: generalize this class / remove from puara_littlefs
{
  DeviceConfiguration& config;
  LITTLEFS& littlefs;

  LittleFSJSONSettings(DeviceConfiguration& config, LITTLEFS& littlefs)
      : config(config)
      , littlefs(littlefs)
  {
  }

  void read_config_json();
  void write_config_json();
  void read_settings_json();
  void write_settings_json();

  void read_config_json_internal(std::string& contents);
  void read_and_process_config_json(DeviceConfiguration& config);

private:
};
}