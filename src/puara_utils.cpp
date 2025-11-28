#include "puara_utils.hpp"
#include "puara_config.hpp"

#include <iostream>

namespace PuaraAPI
{
void find_and_replace(std::string old_text, std::string new_text, std::string& str)
{
  std::size_t old_text_position = str.find(old_text);
  while(old_text_position != std::string::npos)
  {
    str.replace(old_text_position, old_text.length(), new_text);
    old_text_position = str.find(old_text);
  }
  LOG("http (find_and_replace): Success");
}

void find_and_replace(std::string old_text, double new_number, std::string& str)
{
  std::size_t old_text_position = str.find(old_text);
  while(old_text_position != std::string::npos)
  {
    std::string conversion = std::to_string(new_number);
    str.replace(old_text_position, old_text.length(), conversion);
    old_text_position = str.find(old_text);
  }
  LOG("http (find_and_replace): Success");
}

void find_and_replace(std::string old_text, unsigned int new_number, std::string& str)
{
  std::size_t old_text_position = str.find(old_text);
  while(old_text_position != std::string::npos)
  {
    std::string conversion = std::to_string(new_number);
    str.replace(old_text_position, old_text.length(), conversion);
    old_text_position = str.find(old_text);
  }
  LOG("http (find_and_replace): Success");
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
    LOG("http (checkmark): Success");
  }
  else
  {
    LOG("http (checkmark): Could not find the requested string");
  }
}

std::string convertToString(char* a)
{
  std::string s(a);
  return s;
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
        if(i[1] && i[2])
        {
          char hs[]{i[1], i[2]};
          escaped += static_cast<char>(strtol(hs, nullptr, 16));
          i += 2;
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