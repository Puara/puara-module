#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <string>

namespace PuaraAPI
{

void find_and_replace(std::string old_text, std::string new_text, std::string& str);
void find_and_replace(std::string old_text, double new_number, std::string& str);
void find_and_replace(std::string old_text, unsigned int new_number, std::string& str);
void checkmark(std::string old_text, bool value, std::string& str);
std::string urlDecode(std::string text);

template <auto Method, typename T>
void createTask(T* object, std::string_view name, uint32_t stack_depth)
{
  xTaskCreate(+[](void* p) {
    (static_cast<T*>(p)->*Method)();
  }, name.data(), stack_depth, object, 10, NULL);
}
} 