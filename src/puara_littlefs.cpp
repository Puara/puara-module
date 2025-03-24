#include "puara_littlefs.hpp"

#include "puara_config.hpp"

#include <cJSON.h>

#include <fstream>
#include <iomanip>
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
    std::cout << "LittleFS: mount failed" << std::endl;
    return;
  }
  std::cout << "LittleFS: mount successful" << std::endl;
}

void LITTLEFS::unmount_littlefs()
{
  std::cout << "LittleFS: unmounting FS" << std::endl;
  LittleFS.end();
  std::cout << "LittleFS: unmounted" << std::endl;
}

std::string LITTLEFS::read_file(const char* path)
{
  std::cout << "LittleFS: reading file: " << path << std::endl;

  if(!LittleFS.exists(path))
  {
    std::cout << "LittleFS: file not found: " << path << std::endl;
    return "";
  }

  File file = LittleFS.open(path, "r"); //open file
  std::string contents;

  if(!file)
  {
    std::cout << "LittleFS: failed to open file: " << path << std::endl;
    return "";
  }

  while(file.available())
  {
    contents += (char)file.read();
  }

  file.close();

  return contents;
}

void LittleFSJSONSettings::read_config_json()
{
  std::cout << "LittleFS:json: Mounting FS" << std::endl;
  littlefs.mount_littlefs();

  std::cout << "json: Opening config json file" << std::endl;

  File file = LittleFS.open("/config.json", "r"); //littleFS requires the
  if(!file)
  {
    std::cout << "json: Failed to open file" << std::endl;
    littlefs.unmount_littlefs();
    return;
  }

  std::cout << "json: Reading json file" << std::endl;
  std::string contents;
  while(file.available())
  {
    contents += (char)file.read();
  }

  read_config_json_internal(contents);

  file.close();
  littlefs.unmount_littlefs();
}
void LittleFSJSONSettings::read_config_json_internal(std::string& contents)
{
  std::cout << "json: Getting data" << std::endl;
  cJSON* root = cJSON_Parse(contents.c_str());
  if(cJSON_GetObjectItem(root, "device"))
  {
    config.device = cJSON_GetObjectItem(root, "device")->valuestring;
  }
  if(cJSON_GetObjectItem(root, "id"))
  {
    config.id = cJSON_GetObjectItem(root, "id")->valueint;
  }
  if(cJSON_GetObjectItem(root, "author"))
  {
    config.author = cJSON_GetObjectItem(root, "author")->valuestring;
  }
  if(cJSON_GetObjectItem(root, "institution"))
  {
    config.institution = cJSON_GetObjectItem(root, "institution")->valuestring;
  }
  if(cJSON_GetObjectItem(root, "APpasswd"))
  {
    config.APpasswd = cJSON_GetObjectItem(root, "APpasswd")->valuestring;
  }
  if(cJSON_GetObjectItem(root, "wifiSSID"))
  {
    config.wifiSSID = cJSON_GetObjectItem(root, "wifiSSID")->valuestring;
  }
  if(cJSON_GetObjectItem(root, "wifiPSK"))
  {
    config.wifiPSK = cJSON_GetObjectItem(root, "wifiPSK")->valuestring;
  }
  if(cJSON_GetObjectItem(root, "persistentAP"))
  {
    config.persistentAP = cJSON_GetObjectItem(root, "persistentAP")->valueint;
  }
  if(cJSON_GetObjectItem(root, "oscIP1"))
  {
    config.oscIP1 = cJSON_GetObjectItem(root, "oscIP1")->valuestring;
  }
  if(cJSON_GetObjectItem(root, "oscPORT1"))
  {
    config.oscPORT1 = cJSON_GetObjectItem(root, "oscPORT1")->valueint;
  }
  if(cJSON_GetObjectItem(root, "oscIP2"))
  {
    config.oscIP2 = cJSON_GetObjectItem(root, "oscIP2")->valuestring;
  }
  if(cJSON_GetObjectItem(root, "oscPORT2"))
  {
    config.oscPORT2 = cJSON_GetObjectItem(root, "oscPORT2")->valueint;
  }
  if(cJSON_GetObjectItem(root, "localPORT"))
  {
    config.localPORT = cJSON_GetObjectItem(root, "localPORT")->valueint;
  }

  std::cout << "\njson: Data collected:\n\n"
            << "device: " << config.device << "\n"
            << "id: " << config.id << "\n"
            << "author: " << config.author << "\n"
            << "institution: " << config.institution << "\n"
            << "APpasswd: " << config.APpasswd << "\n"
            << "wifiSSID: " << config.wifiSSID << "\n"
            << "wifiPSK: " << config.wifiPSK << "\n"
            << "persistentAP: " << config.persistentAP << "\n"
            << "oscIP1: " << config.oscIP1 << "\n"
            << "oscPORT1: " << config.oscPORT1 << "\n"
            << "oscIP2: " << config.oscIP2 << "\n"
            << "oscPORT2: " << config.oscPORT2 << "\n"
            << "localPORT: " << config.localPORT << "\n"
            << std::endl;

  cJSON_Delete(root);

  std::stringstream tempBuf;
  tempBuf << config.device << "_" << std::setfill('0') << std::setw(3) << config.id;
  config.dmiName = tempBuf.str();
  printf("Device unique name defined: %s\n", config.dmiName.c_str());
}

void LittleFSJSONSettings::read_and_process_config_json(DeviceConfiguration& config)
{
  std::cout << "LittleFS:json: Mounting FS" << std::endl;
  littlefs.mount_littlefs();

  std::cout << "json: Opening settings json file" << std::endl;
  std::string contents = littlefs.read_file("/config.json");

  if(!contents.empty())
  {
    read_config_json_internal(contents);
  }

  littlefs.unmount_littlefs();
}

// copied from puara_spiffs.cpp
void LittleFSJSONSettings::write_config_json() { }

void LittleFSJSONSettings::read_settings_json() { }

void LittleFSJSONSettings::write_settings_json() { }

}