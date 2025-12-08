#include "puara_littlefs.hpp"
#include "puara_config.hpp"

#if !defined(PUARA_SPIFFS)

namespace PuaraAPI
{
void PuaraFileSystem::mount()
{
  LOG("LittleFS: mounting FS");
  if(!LittleFS.begin(true))
  {
    LOG("LittleFS: mount failed");
    return;
  }
  LOG("LittleFS: mount successful");
}

void PuaraFileSystem::unmount()
{
  LOG("LittleFS: unmounting FS");
  LittleFS.end();
  LOG("LittleFS: unmounted");
}

std::string PuaraFileSystem::read_file(std::string_view path)
{
  mount();
  auto path_c = std::string(path).c_str();
  LOG("LittleFS: reading file: ");
  LOG(path_c);
  if(!LittleFS.exists(path_c))
  {
    LOG("LittleFS: file not found: ");
    LOG(path_c);
    return "";
  }

  File file = LittleFS.open(path_c, "r"); //open file
  std::string contents;

  if(!file)
  {
    LOG("LittleFS: failed to open file: ");
    LOG(path_c);
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
  LOG("littleFS: Writing file ");
  LOG(path.c_str());

  File file = LittleFS.open(path.c_str(), FILE_WRITE);
  if (!file) {
    LOG("LittleFS: failed to open file: ");
    LOG(path.c_str());
    return;
  }
  if (file.print(contents.c_str())) {
    LOG("LittleFS: wrote ");
    LOG(path.c_str());
    LOG("closing");
  } else {
    LOG("LittleFS: failed to write ");
    LOG(path.c_str());
    LOG("closing");
  }
  file.close();
  unmount();
}
}
#endif