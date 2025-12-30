#include "puara_filesystem.hpp"

#include "puara_config.hpp"
#include "puara_logger.hpp"

#include <cJSON.h>

#include <fstream>
#include <iomanip>
#include <iostream>

namespace PuaraAPI
{
//// CONFIG ////

// Can be improved
double JSONSettings::getVarNumber(std::string varName)
{
  return variables.at(variables_fields.at(varName)).numberValue;
}

std::string JSONSettings::getVarText(std::string varName)
{
  return variables.at(variables_fields.at(varName)).textValue;
}

void JSONSettings::read_config_json()
{ // Deserialize

  std::string contents = fs.read_file("/config.json");
  read_config_json_internal(contents);
}

void JSONSettings::read_config_json_internal(std::string& contents)
{
  LOG("json: Getting data");
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

  std::cout << "device: " << config.device << "\n"
            << "id: " << config.id << "\n"
            << "author: " << config.author << "\n"
            << "institution: " << config.institution << "\n"
            << "APpasswd: " << config.APpasswd << "\n"
            << "wifiSSID: " << config.wifiSSID << "\n"
            << "wifiPSK: " << config.wifiPSK << "\n"
            << "persistentAP: " << config.persistentAP << "\n"
            << std::endl;

  cJSON_Delete(root);

  std::stringstream tempBuf;
  tempBuf << config.device << "_" << std::setfill('0') << std::setw(3) << config.id;
  config.dmiName = tempBuf.str();
  std::cout << "Device unique name defined: " << config.dmiName.c_str() << std::endl;
}

void JSONSettings::read_settings_json()
{
  std::string contents = fs.read_file("/settings.json");
  read_settings_json_internal(contents);
}

void JSONSettings::read_settings_json_internal(std::string& contents, bool merge)
{
  // Now parse the JSON
  LOG("json: Getting data");
  cJSON* root = cJSON_Parse(contents.c_str());
  cJSON* setting = NULL;
  cJSON* settings = NULL;

  LOG("json: Parse settings information");
  settings = cJSON_GetObjectItemCaseSensitive(root, "settings");

  settingsVariables temp;
  if(!merge)
  {
    variables.clear();
  }
  LOG("json: Extract info");
  cJSON_ArrayForEach(setting, settings)
  {
    cJSON* name = cJSON_GetObjectItemCaseSensitive(setting, "name");
    cJSON* value = cJSON_GetObjectItemCaseSensitive(setting, "value");
    temp.name = name->valuestring;
    if(!cJSON_IsNumber(value))
    {
      temp.textValue = value->valuestring;
      temp.type = "text";
      temp.numberValue = 0;
    }
    else
    {
      temp.textValue.empty();
      temp.numberValue = value->valuedouble;
      temp.type = "number";
    }
    if(variables_fields.find(temp.name) == variables_fields.end())
    {
      variables_fields.insert({temp.name, variables.size()});
      variables.push_back(temp);
    }
    else
    {
      int variable_index = variables_fields.at(temp.name);
      variables.at(variable_index) = temp;
    }
  }

  // Print acquired data
  std::cout << "\nModule-specific settings:\n\n";
  for(auto it : variables)
  {
    std::cout << it.name << ": ";
    if(it.type == "text")
    {
      std::cout << it.textValue << "\n";
    }
    else if(it.type == "number")
    {
      std::cout << it.numberValue << "\n";
    }
  }
  std::cout << std::endl;

  cJSON_Delete(root);
}

void JSONSettings::write_config_json()
{

  cJSON* device_json = NULL;
  cJSON* id_json = NULL;
  cJSON* author_json = NULL;
  cJSON* institution_json = NULL;
  cJSON* APpasswd_json = NULL;
  cJSON* wifiSSID_json = NULL;
  cJSON* wifiPSK_json = NULL;
  cJSON* persistentAP_json = NULL;

  cJSON* root = cJSON_CreateObject();

  device_json = cJSON_CreateString(config.device.c_str());
  cJSON_AddItemToObject(root, "device", device_json);

  id_json = cJSON_CreateNumber(config.id);
  cJSON_AddItemToObject(root, "id", id_json);

  author_json = cJSON_CreateString(config.author.c_str());
  cJSON_AddItemToObject(root, "author", author_json);

  institution_json = cJSON_CreateString(config.institution.c_str());
  cJSON_AddItemToObject(root, "institution", institution_json);

  APpasswd_json = cJSON_CreateString(config.APpasswd.c_str());
  cJSON_AddItemToObject(root, "APpasswd", APpasswd_json);

  wifiSSID_json = cJSON_CreateString(config.wifiSSID.c_str());
  cJSON_AddItemToObject(root, "wifiSSID", wifiSSID_json);

  wifiPSK_json = cJSON_CreateString(config.wifiPSK.c_str());
  cJSON_AddItemToObject(root, "wifiPSK", wifiPSK_json);

  persistentAP_json = cJSON_CreateNumber(config.persistentAP);
  cJSON_AddItemToObject(root, "persistentAP", persistentAP_json);


  std::cout << "\njson: Data stored:\n"
            << "\ndevice: " << config.device << "\n"
            << "id: " << config.id << "\n"
            << "author: " << config.author << "\n"
            << "institution: " << config.institution << "\n"
            << "APpasswd: " << config.APpasswd << "\n"
            << "wifiSSID: " << config.wifiSSID << "\n"
            << "wifiPSK: " << config.wifiPSK << "\n"
            << "persistentAP: " << config.persistentAP << "\n"
            << std::endl;

  // Save to config.json
  LOG("write_config_json: Serializing json");
  std::string contents = cJSON_Print(root);

  fs.write_file("config.json", contents);

  LOG("write_config_json: Delete json entity");
  cJSON_Delete(root);

}

void JSONSettings::write_settings_json()
{
  cJSON* root = cJSON_CreateObject();
  cJSON* settings = cJSON_CreateArray();
  cJSON* setting = NULL;
  cJSON* data = NULL;
  cJSON_AddItemToObject(root, "settings", settings);

  for(auto it : variables)
  {
    setting = cJSON_CreateObject();
    cJSON_AddItemToArray(settings, setting);
    data = cJSON_CreateString(it.name.c_str());
    cJSON_AddItemToObject(setting, "name", data);
    if(it.type == "text")
    {
      data = cJSON_CreateString(it.textValue.c_str());
    }
    else if(it.type == "number")
    {
      data = cJSON_CreateNumber(it.numberValue);
    }
    cJSON_AddItemToObject(setting, "value", data);
  }

  // Save to settings.json
  LOG("write_settings_json: Serializing json");
  std::string contents = cJSON_Print(root);
  LOG("Filesystem: Saving file");

  fs.write_file("/settings.json", contents);

  LOG("write_settings_json: Delete json entity");
  cJSON_Delete(root);

  if(this->on_settings_changed)
    this->on_settings_changed();
}

void JSONSettings::set_settings_changed_handler(std::function<void()> func)
{
  this->on_settings_changed = std::move(func);
}

}
