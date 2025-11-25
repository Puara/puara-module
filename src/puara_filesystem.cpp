#include "puara_filesystem.hpp"

#include "puara_config.hpp"

#include <cJSON.h>

#include <fstream>
#include <iomanip>
#include <iostream>

namespace PuaraAPI
{

static constexpr uint8_t spiffs_max_files = 10;
static constexpr bool spiffs_format_if_mount_failed = false;

void LITTLEFS::mount()
{
  std::cout << "LittleFS: mounting FS" << std::endl;
  if(!LittleFS.begin(true))
  {
    std::cout << "LittleFS: mount failed" << std::endl;
    return;
  }
  std::cout << "LittleFS: mount successful" << std::endl;
}

void LITTLEFS::unmount()
{
  std::cout << "LittleFS: unmounting FS" << std::endl;
  LittleFS.end();
  std::cout << "LittleFS: unmounted" << std::endl;
}

std::string LITTLEFS::read_file(const std::string& path)
{
  mount();
  std::string path_with_slash = "/" + path;
  std::cout << "LittleFS: reading file: " << path << std::endl;
  auto path_c = path_with_slash.c_str();
  if(!LittleFS.exists(path_c))
  {
    std::cout << "LittleFS: file not found: " << path_c << std::endl;
    return "";
  }

  File file = LittleFS.open(path_c, "r"); //open file
  std::string contents;

  if(!file)
  {
    std::cout << "LittleFS: failed to open file: " << path_c << std::endl;
    return "";
  }

  while(file.available())
  {
    contents += (char)file.read();
  }

  file.close();
  unmount();
  return contents;
}

void LITTLEFS::write_file(const std::string& path, const std::string& contents) {
  mount();
  std::cout << "littleFS: Writing file " << path << std::endl;

  File file = LittleFS.open(path.c_str(), FILE_WRITE);
  if (!file) {
    std::cout << "LittleFS: failed to open file: " << path << std::endl;
    return;
  }
  if (file.print(contents.c_str())) {
    std::cout << "LittleFS: wrote "  << path << ", closing" << std::endl;
  } else {
    std::cout << "LittleFS: failed to write "  << path << ", closing" << std::endl;
  }
  file.close();
  unmount();
}

void SPIFFS::mount()
{
  if(!esp_spiffs_mounted(spiffs_config.partition_label))
  {
    std::cout << "spiffs: Initializing SPIFFS" << std::endl;

    spiffs_config.base_path = this->spiffs_base_path.c_str();
    spiffs_config.max_files = PuaraAPI::spiffs_max_files;
    spiffs_config.partition_label = NULL;
    spiffs_config.format_if_mount_failed = PuaraAPI::spiffs_format_if_mount_failed;

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&spiffs_config);

    if(ret != ESP_OK)
    {
      if(ret == ESP_FAIL)
      {
        std::cout << "spiffs: Failed to mount or format filesystem" << std::endl;
      }
      else if(ret == ESP_ERR_NOT_FOUND)
      {
        std::cout << "spiffs: Failed to find SPIFFS partition" << std::endl;
      }
      else
      {
        std::cout << "spiffs: Failed to initialize SPIFFS (" << esp_err_to_name(ret)
                  << ")" << std::endl;
      }
      return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(spiffs_config.partition_label, &total, &used);
    if(ret != ESP_OK)
    {
      std::cout << "spiffs: Failed to get SPIFFS partition information ("
                << esp_err_to_name(ret) << ")" << std::endl;
    }
    else
    {
      std::cout << "spiffs: Partition size: total: " << total << ", used: " << used
                << std::endl;
    }
  }
  else
  {
    std::cout << "spiffs: SPIFFS already initialized" << std::endl;
  }
}

void SPIFFS::unmount()
{
  // All done, unmount partition and disable SPIFFS
  if(esp_spiffs_mounted(spiffs_config.partition_label))
  {
    esp_vfs_spiffs_unregister(spiffs_config.partition_label);
    std::cout << "spiffs: SPIFFS unmounted" << std::endl;
  }
  else
  {
    std::cout << "spiffs: SPIFFS not found" << std::endl;
  }
}

std::string SPIFFS::read_file(const std::string& path) {
  mount();
  std::string path_with_slash = "/" + path;
  std::ifstream in(spiffs_base_path + path_with_slash);
  if (!in) {
    std::cout << "spiffs: Failed to open " << path_with_slash << std::endl;
    return "";
  }
  std::cout << "spiffs: Reading " << path_with_slash <<  std::endl;
  return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  unmount();
}

// TODO: the body is the body of read_file.
void SPIFFS::write_file(const std::string& path, const std::string& contents) {
  mount();
  std::cout << "SPIFFS: Opening " << path << std::endl;
  FILE* f = fopen((spiffs_base_path + path).c_str(), "w");
  if(!f) {
      std::cout << "SPIFFS: Failed to open " << path << std::endl;
      return;
    }

  fprintf(f, "%s", contents.c_str());
  std::cout << "SPIFFS: wrote "  << path << ", closing" << std::endl;
  fclose(f);
  unmount();
}

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

  std::string contents = fs->read_file("config.json");
  read_config_json_internal(contents);
}

void JSONSettings::read_config_json_internal(std::string& contents)
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

  std::cout << "\njson: Data collected:\n\n"
            << "device: " << config.device << "\n"
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
  printf("Device unique name defined: %s\n", config.dmiName.c_str());
}

void JSONSettings::read_settings_json()
{
  std::string contents = fs->read_file("settings.json");
  read_settings_json_internal(contents);
}

void JSONSettings::read_settings_json_internal(std::string& contents, bool merge)
{
  // Now parse the JSON
  std::cout << "json: Getting data" << std::endl;
  cJSON* root = cJSON_Parse(contents.c_str());
  cJSON* setting = NULL;
  cJSON* settings = NULL;

  std::cout << "json: Parse settings information" << std::endl;
  settings = cJSON_GetObjectItemCaseSensitive(root, "settings");

  settingsVariables temp;
  if(!merge)
  {
    variables.clear();
  }
  std::cout << "json: Extract info" << std::endl;
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
  std::cout << "write_config_json: Serializing json" << std::endl;
  std::string contents = cJSON_Print(root);

  fs->write_file("/config.json", contents);

  std::cout << "write_config_json: Delete json entity" << std::endl;
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
  std::cout << "write_settings_json: Serializing json" << std::endl;
  std::string contents = cJSON_Print(root);
  std::cout << "SPIFFS: Saving file" << std::endl;

  fs->write_file("/settings.json", contents);

  std::cout << "write_settings_json: Delete json entity" << std::endl;
  cJSON_Delete(root);

}

}
