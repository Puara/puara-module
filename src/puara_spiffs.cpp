#include "puara_spiffs.hpp"

#include "puara_config.hpp"
#if defined(PUARA_SPIFFS)

#include <fstream>

namespace PuaraAPI
{
static constexpr uint8_t spiffs_max_files = 10;
static constexpr bool spiffs_format_if_mount_failed = false;

void PuaraFileSystem::mount()
{
  if(!esp_spiffs_mounted(spiffs_config.partition_label))
  {
    LOG("spiffs: Initializing SPIFFS");

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
        LOG("spiffs: Failed to mount or format filesystem");
      }
      else if(ret == ESP_ERR_NOT_FOUND)
      {
        LOG("spiffs: Failed to find SPIFFS partition");
      }
      else
      {
        LOG("spiffs: Failed to initialize SPIFFS :");
        LOG(esp_err_to_name(ret));
      }
      return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(spiffs_config.partition_label, &total, &used);
    if(ret != ESP_OK)
    {
      LOG("spiffs: Failed to get SPIFFS partition information");
      LOG(esp_err_to_name(ret));
    }
    else
    {
      LOG("spiffs: Partition size: total: ");
      LOG(std::to_string(total).c_str());
      LOG("used: ");
      LOG(std::to_string(used).c_str());
    }
  }
  else
  {
    LOG("spiffs: SPIFFS already initialized");
  }
}

void PuaraFileSystem::unmount()
{
  // All done, unmount partition and disable SPIFFS
  if(esp_spiffs_mounted(spiffs_config.partition_label))
  {
    esp_vfs_spiffs_unregister(spiffs_config.partition_label);
    LOG("spiffs: SPIFFS unmounted");
  }
  else
  {
    LOG("spiffs: SPIFFS not found");
  }
}

std::string PuaraFileSystem::read_file(std::string_view path)
{
  mount();
  std::string full_path = spiffs_base_path;
  full_path.append(path);
  std::ifstream in(full_path);
  if(!in)
  {
    LOG("spiffs: Failed to open ");
    LOG(full_path.c_str());
    return "";
  }
  LOG("spiffs: Reading ");
  LOG(full_path.c_str());
  std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  unmount();
  return content;
}

// TODO: the body is the body of read_file.
void PuaraFileSystem::write_file(const std::string& path, const std::string& contents)
{
  mount();
  LOG("SPIFFS: Opening ");
  LOG(path.c_str());
  FILE* f = fopen((spiffs_base_path + path).c_str(), "w");
  if(!f)
  {
    LOG("SPIFFS: Failed to open ");
    LOG(path.c_str());
    return;
  }

  fprintf(f, "%s", contents.c_str());
  LOG("SPIFFS: wrote ");
  LOG(path.c_str());
  LOG("closing");
  fclose(f);
  unmount();
}
}
#endif