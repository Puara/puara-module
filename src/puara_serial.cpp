#include "puara_serial.hpp"

#include "puara_config.hpp"
#include "puara_device.hpp"
#include "puara_filesystem.hpp"
#include "puara_utils.hpp"
#include "puara_logger.hpp"

#if defined(PUARA_SPIFFS)
#include "puara_spiffs.hpp"
#else
#include "puara_littlefs.hpp"
#endif

#include <driver/uart.h>
#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#include <driver/usb_serial_jtag.h> // jtag module
#endif

#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3  // ??
#include "esp32-hal-tinyusb.h"
#endif

#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>

namespace PuaraAPI
{
static constexpr std::string_view data_start = "<<<";
static constexpr std::string_view data_end = ">>>";

void Serial::send_serial_data(std::string data)
{
  std::cout << PuaraAPI::data_start << data << PuaraAPI::data_end << std::endl;
}

void Serial::interpret_serial()
{
  while(1)
  {
    vTaskDelay(1000 / portTICK_RATE_MS);
    if(serial_data_str.empty())
    {
      continue;
    }
    if(serial_data_str.compare("reset") == 0 || serial_data_str.compare("reboot") == 0)
    {
      std::cout << "\nRebooting...\n" << std::endl;
      createTask<&Device::reboot_with_delay>(&device, "reboot_with_delay", 1024);
    }
    else if(serial_data_str.compare("ping") == 0)
    {
      std::cout << "pong\n";
    }
    else if(serial_data_str.compare("whatareyou") == 0)
    {
      this->send_serial_data(config.dmiName);
    }
    else if(serial_data_str.rfind("sendconfig", 0) == 0)
    {
      serial_data_str_buffer = serial_data_str.substr(serial_data_str.find(" ") + 1);
      settings.read_config_json_internal(serial_data_str_buffer);
    }
    else if(serial_data_str.rfind("writeconfig") == 0)
    {
      settings.write_config_json();
    }
    else if(serial_data_str.compare("readconfig") == 0)
    {
      std::string contents = fs.read_file("/config.json");
      this->send_serial_data(contents);
    }
    else if(serial_data_str.rfind("sendsettings", 0) == 0)
    {
      serial_data_str_buffer = serial_data_str.substr(serial_data_str.find(" ") + 1);
      settings.read_settings_json_internal(serial_data_str_buffer, true);
    }
    else if(serial_data_str.rfind("writesettings") == 0)
    {
      settings.write_settings_json();
    }
    else if(serial_data_str.compare("readsettings") == 0)
    {
      std::string contents = fs.read_file("/settings.json");
      this->send_serial_data(contents);
    }
    else
    {
      std::cout << "\nI don´t recognize the command \"" << serial_data_str << "\""
                << std::endl;
    }
    serial_data_str.clear();
  }
}

/// REFACTOR: this is related to logging & debugging

void Serial::uart_monitor()
{
  const uart_port_t uart_num0 = UART_NUM_0; // UART port 0
  uart_config_t uart_config0 = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // UART_HW_FLOWCTRL_CTS_RTS,
      .rx_flow_ctrl_thresh = 122,
      .source_clk = UART_SCLK_APB,
  };

  // Configure UART1 parameters
  uart_param_config(uart_num0, &uart_config0);

  uart_set_pin(
      uart_num0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
      UART_PIN_NO_CHANGE);

  // Install UART driver (we don't need an event queue here)
  // In this example we don't even use a buffer for sending data.
  if(uart_is_driver_installed(uart_num0))
  {
    LOG("UART driver already installed. Skipping installation.");
  }
  else
  {
    esp_err_t err = uart_driver_install(uart_num0, UART_FIFO_LEN + 1, 0, 0, NULL, 0);
    if (err != ESP_OK)
    {
      LOG("Failed to install UART driver. Error code: ");
      LOG(EspErr{err});
      return;
    }
  }

  while(1)
  {
    // Read data from UART
    serial_data_length = uart_read_bytes(
        uart_num0, serial_data, PUARA_SERIAL_BUFSIZE, 500 / portTICK_RATE_MS);
    if(serial_data_length > 0)
    {
      serial_data_str = serial_data;
      memset(serial_data, 0, sizeof serial_data);
      uart_flush(uart_num0);
    }
  }
}

void Serial::jtag_monitor()
{
#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
  // Setup jtag module for USB Serial reads
  usb_serial_jtag_driver_config_t jtag_config{
      .tx_buffer_size = 256,
      .rx_buffer_size = 256,
  };

  // Install jtag module
  usb_serial_jtag_driver_install(&jtag_config);

  while(1)
  {
    // serial_data_length = USBSerial.read();
    // Only read if connected to PC
    serial_data_length = usb_serial_jtag_read_bytes(
        serial_data, PUARA_SERIAL_BUFSIZE, 500 / portTICK_RATE_MS);
    if(serial_data_length > 0)
    {
      serial_data_str = serial_data;
      // remove new line character at end
      if(serial_data_str[serial_data_str.size() - 1] == '\n')
        serial_data_str.erase(serial_data_str.size() - 1);
      if(serial_data_str[serial_data_str.size() - 1] == '\r')
        serial_data_str.erase(serial_data_str.size() - 1);
      memset(serial_data, 0, sizeof serial_data);
    }
  }
#endif
}

void Serial::usb_monitor()
{
#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
  // // Setup usb module for USB reads
  // const char *product_name = dmiName.c_str();
  // const char *manufacturer_name = author.c_str();

  // tinyusb_device_config_t usb_config = {
  //     .vid = USB_ESPRESSIF_VID,
  //     .pid = 0x0002,
  //     .product_name = product_name,
  //     .manufacturer_name = manufacturer_name,
  //     .serial_number = product_name,
  //     .fw_version = version,
  //     .usb_version = 0x0200,
  //     .usb_class = TUSB_CLASS_MISC,
  //     .usb_subclass = MISC_SUBCLASS_COMMON,
  //     .usb_protocol = MISC_PROTOCOL_IAD,
  //     .usb_attributes = TUSB_DESC_CONFIG_ATT_SELF_POWERED,
  //     .usb_power_ma = 500,
  //     .webusb_enabled = false,
  //     .webusb_url = "espressif.github.io/arduino-esp32/webusb.html"
  // };

  // // Setup USB interface
  // tinyusb_init(&usb_config);
  // TODO: Read from USB interface
  LOG("USB OTG monitor not supported, use the USB Serial JTAG or UART interface");
#endif
}

bool Serial::start_serial_listening()
{
  if(module_monitor == UART_MONITOR)
  { 
    createTask<&Serial::uart_monitor>(this, "serial_monitor", 2048);
    createTask<&Serial::interpret_serial>(this, "interpret_serial", 4096);
  }
  else if(module_monitor == JTAG_MONITOR)
  {
    createTask<&Serial::jtag_monitor>(this, "serial_monitor", 2048);
    createTask<&Serial::interpret_serial>(this, "interpret_serial", 4096);
  }
  else if(module_monitor == USB_MONITOR)
  {
    createTask<&Serial::usb_monitor>(this, "serial_monitor", 2048);
    createTask<&Serial::interpret_serial>(this, "interpret_serial", 4096);
  }
  else
  {
    LOG("Invalid Monitor Type");
  }
  return 1;
}
}
