#pragma once
#include <esp_spi_flash.h>
#include <esp_spiffs.h>

#include <fstream>
#include <string>

namespace PuaraAPI
{
class PuaraFileSystem
{
public:
  void mount();
  void unmount();
  std::string read_file(const std::string& path);
  void write_file(const std::string& path, const std::string& content);

private:
  esp_vfs_spiffs_conf_t spiffs_config;
  std::string spiffs_base_path = "/spiffs";
};
}