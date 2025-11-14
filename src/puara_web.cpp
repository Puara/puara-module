#include "puara_web.hpp"

#include "puara_config.hpp"
#include "puara_device.hpp"
#include "puara_filesystem.hpp"
#include "puara_utils.hpp"
#include "puara_wifi.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>

namespace PuaraAPI
{

httpd_handle_t Webserver::start_webserver(void)
{
  if(!wifi.ApStarted)
  {
    std::cout << "start_webserver: Cannot start webserver: AP and STA not initializated"
              << std::endl;
    return NULL;
  }
  this->webserver = NULL;

  this->webserver_config.task_priority = tskIDLE_PRIORITY + 5;
  this->webserver_config.stack_size = 4096;
  this->webserver_config.core_id = tskNO_AFFINITY;
  this->webserver_config.server_port = 80;
  this->webserver_config.ctrl_port = 32768;
  this->webserver_config.max_open_sockets = 7;
  this->webserver_config.max_uri_handlers = 9;
  this->webserver_config.max_resp_headers = 9;
  this->webserver_config.backlog_conn = 5;
  this->webserver_config.lru_purge_enable = true;
  this->webserver_config.recv_wait_timeout = 5;
  this->webserver_config.send_wait_timeout = 5;
  this->webserver_config.global_user_ctx = NULL;
  this->webserver_config.global_user_ctx_free_fn = NULL;
  this->webserver_config.global_transport_ctx = NULL;
  this->webserver_config.global_transport_ctx_free_fn = NULL;
  this->webserver_config.open_fn = NULL;
  this->webserver_config.close_fn = NULL;
  this->webserver_config.uri_match_fn = NULL;

#define make_http_func(func_name, url)                  \
  +[](httpd_req_t* req) -> esp_err_t {                  \
    auto self = static_cast<Webserver*>(req->user_ctx); \
    req->user_ctx = (char*)url;                         \
    return self->func_name(req);                        \
  }
  this->index.uri = "/";
  this->index.method = HTTP_GET;
  this->index.handler = make_http_func(index_get_handler, "index.html");
  this->index.user_ctx = this;

  this->indexpost.uri = "/";
  this->indexpost.method = HTTP_POST;
  this->indexpost.handler = make_http_func(index_post_handler, "index.html");
  this->indexpost.user_ctx = this;

  this->style.uri = "/style.css";
  this->style.method = HTTP_GET;
  this->style.handler = make_http_func(style_get_handler, "style.css");
  this->style.user_ctx = this;

  // this->factory.uri = "/factory.html";
  // this->factory.method    = HTTP_GET;
  // this->factory.handler   = get_handler;
  // this->factory.user_ctx  = (char*)"factory.html";

  this->reboot.uri = "/reboot.html";
  this->reboot.method = HTTP_GET;
  this->reboot.handler = make_http_func(get_handler, "reboot.html");
  this->reboot.user_ctx = this;

  this->scan.uri = "/scan.html";
  this->scan.method = HTTP_GET;
  this->scan.handler = make_http_func(scan_get_handler, "scan.html");
  this->scan.user_ctx = this;

  // this->update.uri = "/update.html";
  // this->update.method    = HTTP_GET;
  // this->update.handler   = get_handler;
  // this->update.user_ctx  = (char*)"update.html";

  this->settingsget.uri = "/settings.html";
  this->settingsget.method = HTTP_GET;
  this->settingsget.handler
      = make_http_func(settings_get_handler, "settings.html");
  this->settingsget.user_ctx = this;

  this->settingspost.uri = "/settings.html";
  this->settingspost.method = HTTP_POST;
  this->settingspost.handler
      = make_http_func(settings_post_handler, "settings.html");
  this->settingspost.user_ctx = this;

  // Start the httpd server
  std::cout << "webserver: Starting server on port: " << webserver_config.server_port
            << std::endl;
  if(httpd_start(&webserver, &webserver_config) == ESP_OK)
  {
    // Set URI handlers
    std::cout << "webserver: Registering URI handlers" << std::endl;
    httpd_register_uri_handler(webserver, &this->index);
    httpd_register_uri_handler(webserver, &this->indexpost);
    httpd_register_uri_handler(webserver, &this->style);
    httpd_register_uri_handler(webserver, &this->scan);
    // httpd_register_uri_handler(webserver, &this->factory);
    httpd_register_uri_handler(webserver, &this->reboot);
    // httpd_register_uri_handler(webserver, &this->update);
    httpd_register_uri_handler(webserver, &this->settingsget);
    httpd_register_uri_handler(webserver, &this->settingspost);
    return webserver;
  }

  std::cout << "webserver: Error starting server!" << std::endl;
  return NULL;
}

void Webserver::stop_webserver(void)
{
  // Stop the httpd server
  httpd_stop(webserver);
}

std::string Webserver::prepare_index()
{
  std::cout << "http : Reading index file" << std::endl;
  std::string contents = fs->read_file("index.html");
  // Put the module info on the HTML before send response
  find_and_replace("%DMINAME%", config.dmiName, contents);
  if(wifi.StaIsConnected)
  {
    find_and_replace(
        "%STATUS%",
        "Currently connected on "
        "<strong style=\"color:Tomato;\">"
            + config.wifiSSID + "</strong> network",
        contents);
  }
  else
  {
    find_and_replace("%STATUS%", "Currently not connected to any network", contents);
  }
  find_and_replace("%CURRENTSSID%", wifi.currentSSID, contents);
  find_and_replace("%CURRENTPSK%", config.wifiPSK, contents);
  checkmark("%CURRENTPERSISTENT%", config.persistentAP, contents);
  find_and_replace("%DEVICENAME%", config.device, contents);
  find_and_replace("%CURRENTSSID2%", config.wifiSSID, contents);
  find_and_replace("%CURRENTIP%", wifi.currentSTA_IP, contents);
  find_and_replace("%CURRENTAPIP%", wifi.currentAP_IP, contents);
  find_and_replace("%CURRENTSTAMAC%", wifi.currentSTA_MAC, contents);
  find_and_replace("%CURRENTAPMAC%", wifi.currentAP_MAC, contents);
  std::ostringstream tempBuf;
  tempBuf << std::setfill('0') << std::setw(3) << std::hex << config.id;
  find_and_replace("%MODULEID%", tempBuf.str(), contents);
  find_and_replace("%MODULEAUTH%", config.author, contents);
  find_and_replace("%MODULEINST%", config.institution, contents);
  find_and_replace("%MODULEVER%", config.version, contents);

  return contents;
}

esp_err_t Webserver::index_get_handler(httpd_req_t* req)
{
  std::string prepared_index = prepare_index();
  httpd_resp_sendstr(req, prepared_index.c_str());

  return ESP_OK;
}

esp_err_t Webserver::settings_get_handler(httpd_req_t* req)
{
  std::cout << "http : Reading settings file" << std::endl;
  std::string contents = fs->read_file("settings.html");

  std::cout << "settings_get_handler: Adding variables to HTML" << std::endl;
  std::string settings;
  for(const auto& it : this->settings.variables)
  {
    if(it.type == "text")
    {
      settings.append(
          "<div class=\"row\"><div class=\"col-25\"><label "
          "for=\"%PARAMETER%\">%PARAMETER%</label></div><div class=\"col-75\"><input "
          "type=\"text\" "
          "id=\"%PARAMETER%\" name=\"%PARAMETER%\" "
          "value=\"%PARAMETERVALUE%\"></div></div>");
      find_and_replace("%PARAMETERVALUE%", it.textValue, settings);
      find_and_replace("%PARAMETER%", it.name, settings);
    }
    else if(it.type == "number")
    {
      settings.append(
          "<div class=\"row\"><div class=\"col-25\"><label "
          "for=\"%PARAMETER%\">%PARAMETER%</label></div><div class=\"col-75\"><input "
          "type=\"number\" step=\"0.000001\" id=\"%PARAMETER%\" name=\"%PARAMETER%\" "
          "value=\"%PARAMETERVALUE%\"></div></div>");
      find_and_replace("%PARAMETERVALUE%", it.numberValue, settings);
      find_and_replace("%PARAMETER%", it.name, settings);
    }
  }
  find_and_replace("%DATAFROMMODULE%", settings, contents);
  httpd_resp_sendstr(req, contents.c_str());

  return ESP_OK;
}

esp_err_t Webserver::settings_post_handler(httpd_req_t* req)
{
  char buf[200];

  int api_return, remaining = req->content_len;

  while(remaining > 0)
  {
    /* Read the data for the request */
    if((api_return = httpd_req_recv(req, buf, std::min(remaining, (int)sizeof(buf))))
       <= 0)
    {
      if(api_return == HTTPD_SOCK_ERR_TIMEOUT)
      {
        /* Retry receiving if timeout occurred */
        continue;
      }
      return ESP_FAIL;
    }
    std::string str_buf = convertToString(buf);
    std::string str_token;
    std::string field;
    size_t pos = 0;
    size_t field_pos = 0;
    std::string delimiter = "&";
    std::string field_delimiter = "=";
    // adding delimiter to process last variable in the loop
    str_buf.append(delimiter);

    std::cout << "Settings stored:" << std::endl;
    auto& variables = settings.variables;
    auto& variables_fields = settings.variables_fields;
    while((pos = str_buf.find(delimiter)) != std::string::npos)
    {
      str_token = str_buf.substr(0, pos);
      field_pos = str_buf.find(field_delimiter);
      field = str_token.substr(0, field_pos);
      str_token.erase(0, field_pos + field_delimiter.length());
      std::cout << field << ": ";
      if(variables.at(variables_fields.at(field)).type == "text")
      {
        variables.at(variables_fields.at(field)).textValue = urlDecode(str_token);
      }
      else if(variables.at(variables_fields.at(field)).type == "number")
      {
        variables.at(variables_fields.at(field)).numberValue = std::stod(str_token);
      }
      std::cout << str_token << std::endl;
      str_buf.erase(0, pos + delimiter.length());
    }
    std::cout << std::endl;
    remaining -= api_return;
  }

  settings.write_settings_json();
  std::cout << "http : Reading saved.html file" << std::endl;
  std::string contents = fs->read_file("saved.html");

  return ESP_OK;
}

esp_err_t Webserver::get_handler(httpd_req_t* req)
{
  const char* resp_str = (const char*)req->user_ctx;
  std::string requested_path = std::string{resp_str};
  std::cout << "http : Reading requested file " << requested_path << std::endl;
  std::string contents = fs->read_file(requested_path);
  httpd_resp_sendstr(req, contents.c_str());

  return ESP_OK;
}

esp_err_t Webserver::style_get_handler(httpd_req_t* req)
{
  const char* resp_str = (const char*)req->user_ctx;
  std::string requested_path = std::string{resp_str};
  std::cout << "http : Reading style.css file" << std::endl;
  std::string contents = fs->read_file(requested_path);
  httpd_resp_set_type(req, "text/css");
  httpd_resp_sendstr(req, contents.c_str());

  return ESP_OK;
}

esp_err_t Webserver::scan_get_handler(httpd_req_t* req)
{
  const char* resp_str = (const char*)req->user_ctx;
  std::string requested_path = std::string{resp_str};
  std::cout << "http : Reading scan.html file" << std::endl;
  std::ifstream in(resp_str);
  std::string contents = fs->read_file(requested_path);
  wifi.wifi_scan();
  find_and_replace("%SSIDS%", wifi.wifiAvailableSsid, contents);
  httpd_resp_sendstr(req, contents.c_str());

  return ESP_OK;
}

esp_err_t Webserver::index_post_handler(httpd_req_t* req)
{
  char buf[200];
  bool ret_flag = false;

  int api_return, remaining = req->content_len;

  while(remaining > 0)
  {
    /* Read the data for the request */
    if((api_return = httpd_req_recv(req, buf, std::min(remaining, (int)sizeof(buf))))
       <= 0)
    {
      if(api_return == HTTPD_SOCK_ERR_TIMEOUT)
      {
        /* Retry receiving if timeout occurred */
        continue;
      }
      return ESP_FAIL;
    }

    std::string str_buf = convertToString(buf);
    std::string str_token;
    std::string field;
    size_t pos = 0;
    size_t field_pos = 0;
    std::string delimiter = "&";
    std::string field_delimiter = "=";
    // adding delimiter to process last variable in the loop
    str_buf.append(delimiter);
    bool checkbox_persistentAP = false;

    while((pos = str_buf.find(delimiter)) != std::string::npos)
    {
      str_token = str_buf.substr(0, pos);
      field_pos = str_buf.find(field_delimiter);
      field = str_token.substr(0, field_pos);
      str_token.erase(0, field_pos + field_delimiter.length());
      if(config_fields.find(field) != config_fields.end())
      {
        switch(config_fields.at(field))
        {
          case 1:
            std::cout << "SSID: " << str_token << std::endl;
            if(!str_token.empty())
            {
              config.wifiSSID = urlDecode(str_token);
            }
            else
            {
              std::cout << "SSID empty! Keeping the stored value" << std::endl;
            }
            break;
          case 2:
            std::cout << "APpasswd: " << str_token << std::endl;
            if(!str_token.empty())
            {
              this->APpasswdVal1 = urlDecode(str_token);
            }
            else
            {
              std::cout << "APpasswd empty! Keeping the stored value" << std::endl;
              this->APpasswdVal1.clear();
            };
            break;
          case 3:
            std::cout << "APpasswdValidate: " << str_token << std::endl;
            if(!str_token.empty())
            {
              this->APpasswdVal2 = urlDecode(str_token);
            }
            else
            {
              std::cout << "APpasswdValidate empty! Keeping the stored value"
                        << std::endl;
              this->APpasswdVal2.clear();
            };
            break;
          case 4:
            std::cout << "password: " << str_token << std::endl;
            if(!str_token.empty())
            {
              config.wifiPSK = urlDecode(str_token);
            }
            else
            {
              std::cout << "password empty! Keeping the stored value" << std::endl;
            }
            break;
          case 5:
            std::cout << "Rebooting\n";
            ret_flag = true;
            break;
          case 6:
            std::cout << "persistentAP: " << str_token << std::endl;
            checkbox_persistentAP = true;
            break;
          default:
            std::cout << "Error, no match for config field to store received data\n";
            break;
        }
      }
      else
      {
        std::cout << "Error, no match for config field to store received data: " << field
                  << std::endl;
      }
      str_buf.erase(0, pos + delimiter.length());
    }

    // processing some post info
    if(APpasswdVal1 == APpasswdVal2 && !APpasswdVal1.empty()
       && APpasswdVal1.length() > 7)
    {
      config.APpasswd = APpasswdVal1;
      std::cout << "Puara password changed!\n";
    }
    else
    {
      std::cout << "Puara password doesn't match or shorter than 8 characteres. "
                   "Passwork not changed.\n";
    }
    config.persistentAP = checkbox_persistentAP;
    APpasswdVal1.clear();
    APpasswdVal2.clear();
    remaining -= api_return;
  }

  if(ret_flag)
  {
    std::cout << "http : Reading reboot.html file" << std::endl;
    std::string contents = fs->read_file("reboot.html");
    httpd_resp_sendstr(req, contents.c_str());
    std::cout << "\nRebooting...\n" << std::endl;
    createTask<&Device::reboot_with_delay>(&device, "reboot_with_delay", 1024);
  }
  else
  {
    settings.write_config_json();
    std::cout << "http : Reading saved.html file" << std::endl;
    std::string contents = fs->read_file("saved.html");
    httpd_resp_sendstr(req, contents.c_str());
  }

  return ESP_OK;
}
}
