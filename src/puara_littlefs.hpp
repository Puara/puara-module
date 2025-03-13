#pragma once

#include "puara_config.hpp"

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

private:
};
}