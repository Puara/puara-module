#include "puara_mdns.hpp"
#include "puara_config.hpp"

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
    LOG("MDNS Init failed: ");
    LOG(esp_err_to_name(err));
    return;
  }
  // set hostname
  ESP_ERROR_CHECK(mdns_hostname_set(device_name.data()));
  // set default instance
  ESP_ERROR_CHECK(mdns_instance_name_set(instance_name.data()));
  LOG("MDNS Init completed. Device name: ");
  LOG(device_name.data());
  std::cout << "Device wifi network : " << device_name << "\n"
            << "Open device network config/settings in browser with : http://" << device_name << ".local/" 
            << std::endl;
}
} 
