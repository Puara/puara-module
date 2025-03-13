#include "puara_littlefs.hpp"

#include "puara_config.hpp"

#include <LittleFS.h>

#include <iostream>

namespace PuaraAPI
{
void LITTLEFS::config_littlefs()
{
  std::cout << "LittleFS: configuring" << std::endl;
}

void LITTLEFS::mount_littlefs()
{
  std::cout << "LittleFS: mounting FS" << std::endl;
  if(!LittleFS.begin(true))
  {
    std::cout << "LittleFS: mount failed!" << std::endl;
    return;
  }
  std::cout << "LittleFS: mount successful!" << std::endl;
}

void LITTLEFS::unmount_littlefs()
{
  std::cout << "LittleFS: unmounting FS" << std::endl;
  LittleFS.end();
  std::cout << "LittleFS: unmounted" << std::endl;
}
}