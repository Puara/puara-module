#pragma once
#include <string>

#if defined(PUARA_LITTLEFS)
#include "puara_littlefs.hpp"
#else
#include "puara_spiffs.hpp"
#endif

namespace PuaraAPI
{

class FileSystem
{
public:
  FileSystem();

  void config();
  void mount();
  void unmount();

  std::string read(const std::string& path);
  void write(const std::string& path, const std::string& contents);

private:
#if defined(PUARA_LITTLEFS)
  LITTLEFS littlefs;
#else
  SPIFFS spiffs;
#endif
};

}