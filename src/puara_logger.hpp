#pragma once
#include <cstdio>
#include <string>
#include <string_view>
#include <span>
#include <cinttypes>

namespace PuaraAPI
{
template<std::size_t N>
inline void log(const char (&str)[N]) {
  fprintf(stderr, "%s\n", str);
}
inline void log(std::string_view str) {
  fprintf(stderr, "%*s\n", str.size(), str.data());
}
inline void log(const std::string& str) {
  fprintf(stderr, "%s\n", str.c_str());
}
inline void log(unsigned char v) {
  fprintf(stderr, "%d\n", (int)v);
}
inline void log(char v) {
  fprintf(stderr, "%c\n", (int)v);
}
inline void log(int16_t v) {
  fprintf(stderr, "%d\n", (int)v);
}
inline void log(uint16_t v) {
  fprintf(stderr, "%d\n", (int)v);
}
inline void log(int32_t v) {
  fprintf(stderr, "%d\n", v);
}
inline void log(uint32_t v) {
  fprintf(stderr, "%u\n", v);
}
inline void log(int64_t v) {
  fprintf(stderr, "%lld\n", v);
}
inline void log(uint64_t v) {
  fprintf(stderr, "%llu\n", v);
}
inline void log(float v) {
  fprintf(stderr, "%f\n", (double)v);
}
inline void log(double v) {
  fprintf(stderr, "%f\n", v);
}
}

#define PUARA_DEBUG  //uncomment this line to access all LOG statements for debug

#ifdef PUARA_DEBUG
  #define LOG(x) do { ::PuaraAPI::log(x); } while(0)
#else
  #define LOG(x) do {} while(0)
#endif
