#include "puara_littlefs.hpp"
#include "puara_config.hpp"
#include "puara_logger.hpp"

#if !defined(PUARA_SPIFFS)

namespace PuaraAPI
{
void PuaraFileSystem::mount()
{
  ESP_LOGI(PUARA_TAG,"LittleFS: mounting FS");
  if(!LittleFS.begin(true))
  {
    ESP_LOGE(PUARA_TAG,"LittleFS: mount failed");
    return;
  }
  ESP_LOGI(PUARA_TAG,"LittleFS: mount successful");
}

void PuaraFileSystem::unmount()
{
  ESP_LOGI(PUARA_TAG,"LittleFS: unmounting FS");
  LittleFS.end();
  ESP_LOGI(PUARA_TAG,"LittleFS: unmounted");
}

std::string PuaraFileSystem::read_file(std::string_view path)
{
  mount();
  auto path_c = std::string(path).c_str();
  ESP_LOGI(PUARA_TAG,"LittleFS: reading file: ");
  ESP_LOGI(PUARA_TAG,path_c);
  if(!LittleFS.exists(path_c))
  {
    ESP_LOGE(PUARA_TAG,"LittleFS: file not found: %s", path_c);
    return "";
  }

  File file = LittleFS.open(path_c, "r"); //open file
  std::string contents;

  if(!file)
  {
    ESP_LOGE(PUARA_TAG,"LittleFS: failed to open file: %s", path_c);
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

void PuaraFileSystem::write_file(const std::string& path, const std::string& contents) {
  mount();
  ESP_LOGI(PUARA_TAG,"littleFS: Writing file %s", path.c_str());

  File file = LittleFS.open(path.c_str(), FILE_WRITE);
  if (!file) {
    ESP_LOGE(PUARA_TAG,"LittleFS: failed to open file: %s", path.c_str());
    return;
  }
  if (file.print(contents.c_str())) {
    ESP_LOGI(PUARA_TAG,"LittleFS: wrote %s", path.c_str());
  } else {
    ESP_LOGE(PUARA_TAG,"LittleFS: failed to write %s", path.c_str());
  }
  ESP_LOGI(PUARA_TAG,"closing");
  file.close();
  unmount();
}
}
#endif
