#pragma once
#include <esp_err.h>
#include <esp_http_server.h>

#include <string>
#include <unordered_map>

typedef void* httpd_handle_t;
struct httpd_req;
typedef struct httpd_req httpd_req_t;

namespace PuaraAPI
{
struct DeviceConfiguration;
struct Device;
struct SPIFFS;
struct JSONSettings;
struct WiFi;

struct Webserver
{
  DeviceConfiguration& config;
  Device& device;
  SPIFFS& spiffs;
  JSONSettings& settings;
  WiFi& wifi;

  // public API
  httpd_handle_t start_webserver(void);
  void stop_webserver(void);

  // private API
  esp_err_t index_get_handler(httpd_req_t* req);
  esp_err_t get_handler(httpd_req_t* req);
  esp_err_t style_get_handler(httpd_req_t* req);
  esp_err_t settings_get_handler(httpd_req_t* req);
  esp_err_t settings_post_handler(httpd_req_t* req);
  esp_err_t scan_get_handler(httpd_req_t* req);
  esp_err_t index_post_handler(httpd_req_t* req);
  std::string prepare_index();

  httpd_uri_t reboot;
  httpd_uri_t index;
  httpd_uri_t style;
  // httpd_uri_t factory;
  httpd_uri_t scan;
  // httpd_uri_t update;
  httpd_uri_t indexpost;
  httpd_uri_t settingsget;
  httpd_uri_t settingspost;

  std::string APpasswdVal1;
  std::string APpasswdVal2;

  httpd_handle_t webserver;
  httpd_config_t webserver_config;

  // FIXME frozen::hash_map
  std::unordered_map<std::string, int> config_fields
      = {{"SSID", 1},     {"APpasswd", 2}, {"APpasswdValidate", 3},
         {"password", 4}, {"reboot", 5},   {"persistentAP", 6}};
};
}