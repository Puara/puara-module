#include "puara_filesystem.hpp"

#include <fstream>
#include <iostream>

#if defined(PUARA_LITTLEFS)
#include "puara_littlefs.hpp"
#else
#include <esp_spiffs.h>
#endif

namespace PuaraAPI
{

FileSystem::FileSystem() { }

void FileSystem::config()
{
#if defined(PUARA_LITTLEFS)
  littlefs.config_littlefs();
#else
  spiffs.config_spiffs();
#endif
}

void FileSystem::mount()
{
#if defined(PUARA_LITTLEFS)
  littlefs.mount_littlefs();
#else
  spiffs.mount_spiffs();
#endif
}

void FileSystem::unmount()
{
#if defined(PUARA_LITTLEFS)
  littlefs.unmount_littlefs();
#else
  spiffs.unmount_spiffs();
#endif
}

std::string FileSystem::read(const std::string& path)
{
#if defined(PUARA_LITTLEFS)
  if(!LittleFS.exists(path.c_str()))
  {
    std::cout << "LittleFS: file not found: " << path << std::endl;
    return "";
  }

  File file = LittleFS.open(path.c_str(), "r");
  std::string contents;
  while(file.available())
  {
    contents += (char)file.read();
  }
  file.close();
  return contents;
#else
  FILE* f = fopen(path.c_str(), "r");
  if(f == NULL)
  {
    std::cout << "SPIFFS: failed to open: " << path << std::endl;
    return "";
  }

  std::ifstream in(path);
  std::string contents(
      (std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  fclose(f);
  return contents;
#endif
}

void FileSystem::write(const std::string& path, const std::string& contents)
{
#if defined(PUARA_LITTLEFS)
  File file = LittleFS.open(path.c_str(), "w");
  if(!file)
  {
    std::cout << "LittleFS: failed to write: " << path << std::endl;
    return;
  }
  file.print(contents.c_str());
  file.close();
#else
  FILE* f = fopen(path.c_str(), "w");
  if(f == NULL)
  {
    std::cout << "SPIFFS: failed to write: " << path << std::endl;
    return;
  }
  fprintf(f, "%s", contents.c_str());
  fclose(f);
#endif
}

}
