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
  LOG("LittleFS: reading file: " << path_c);
  if(!LittleFS.exists(path_c))
  {
    LOG("LittleFS: file not found: " << path_c);
    return "";
  }

  File file = LittleFS.open(path_c, "r"); //open file
  std::string contents;

  if(!file)
  {
    LOG("LittleFS: failed to open file: " << path_c);
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
  LOG("littleFS: Writing file " << path);

  File file = LittleFS.open(path.c_str(), FILE_WRITE);
  if (!file) {
    LOG("LittleFS: failed to open file: " << path);
    return;
  }
  if (file.print(contents.c_str())) {
    LOG("LittleFS: wrote "  << path << ", closing");
  } else {
    LOG("LittleFS: failed to write "  << path << ", closing");
  }
  file.close();
  unmount();
}
}
#endif