#pragma once
#include <esp_spi_flash.h>

#include <LittleFS.h>
#include <fstream>

namespace PuaraAPI
{
class PuaraFileSystem {
 public:
  void mount();
  void unmount();
  std::string read_file(std::string_view path);
  void write_file(const std::string& path, const std::string& content);
};
}