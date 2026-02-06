#include "puara_mdns.hpp"
#include "puara_config.hpp"
#include "puara_logger.hpp"

#include <mdns.h>

#include <iostream>

namespace PuaraAPI
{
void MDNSService::start(std::string_view device_name, std::string_view instance_name)
{
  // initialize mDNS service
  esp_err_t err = mdns_init();
  if(err)
  {
    ESP_LOGE(PUARA_TAG,"MDNS Init failed. Error code %d ", err);
    return;
  }
  // set hostname
  ESP_ERROR_CHECK(mdns_hostname_set(device_name.data()));
  // set default instance
  ESP_ERROR_CHECK(mdns_instance_name_set(instance_name.data()));
  ESP_LOGI(PUARA_TAG,"MDNS Init completed. Device name: %s", device_name.data());
  std::cout << "Device wifi network : " << device_name << "\n"
            << "Open device network config/settings in browser with : http://" << device_name << ".local/"
            << std::endl;
}
}
