#include "puara_utils.hpp"
#include "puara_config.hpp"
#include "puara_logger.hpp"

#include <iostream>

namespace PuaraAPI
{
void find_and_replace(std::string old_text, std::string new_text, std::string& str)
{
  std::size_t old_text_position = str.find(old_text);
  while(old_text_position != std::string::npos)
  {
    str.replace(old_text_position, old_text.length(), new_text);
    old_text_position = str.find(old_text, old_text_position + new_text.length());
  }
  ESP_LOGV(PUARA_TAG,"http (find_and_replace): Success");
}

void find_and_replace(std::string old_text, double new_number, std::string& str)
{
  std::string conversion = std::to_string(new_number);
  std::size_t old_text_position = str.find(old_text);
  while(old_text_position != std::string::npos)
  {
    str.replace(old_text_position, old_text.length(), conversion);
    old_text_position = str.find(old_text, old_text_position + conversion.length());
  }
  ESP_LOGV(PUARA_TAG,"http (find_and_replace): Success");
}

void find_and_replace(std::string old_text, unsigned int new_number, std::string& str)
{
  std::string conversion = std::to_string(new_number);
  std::size_t old_text_position = str.find(old_text);
  while(old_text_position != std::string::npos)
  {
    str.replace(old_text_position, old_text.length(), conversion);
    old_text_position = str.find(old_text, old_text_position + conversion.length());
  }
  ESP_LOGV(PUARA_TAG,"http (find_and_replace): Success");
}

void checkmark(std::string old_text, bool value, std::string& str)
{
  std::size_t old_text_position = str.find(old_text);
  if(old_text_position != std::string::npos)
  {
    std::string conversion;
    if(value)
    {
      conversion = "checked";
    }
    else
    {
      conversion = "";
    }
    str.replace(old_text_position, old_text.length(), conversion);
    ESP_LOGI(PUARA_TAG,"http (checkmark): Success");
  }
  else
  {
    ESP_LOGW(PUARA_TAG,"http (checkmark): Could not find the requested string");
  }
}

std::string urlDecode(std::string text)
{
  std::string escaped;
  for(auto i = text.begin(), nd = text.end(); i < nd; ++i)
  {
    auto c = (*i);
    switch(c)
    {
      case '%':
        if(i + 2 < nd)
        {
          const char hs[]{i[1], i[2], '\0'};
          escaped += static_cast<char>(strtol(hs, nullptr, 16));
          i += 2;
        }
        else
        {
          escaped += c;
        }
        break;
      case '+':
        escaped += ' ';
        break;
      default:
        escaped += c;
    }
  }
  return escaped;
}
}
