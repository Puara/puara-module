#include "puara.h"

char Puara::serial_data[PUARA_SERIAL_BUFSIZE];
int Puara::serial_data_length;
std::string Puara::serial_data_str;
std::string Puara::serial_data_str_buffer;
int Puara::module_monitor = UART_MONITOR;

// FIXME: refactor into std::string_view
const std::string Puara::data_start = "<<<";
const std::string Puara::data_end = ">>>";

void Puara::send_serial_data(std::string data) {
  std::cout << Puara::data_start << data << Puara::data_end << std::endl;
}

void Puara::interpret_serial(void* pvParameters) {
  while (1) {
    vTaskDelay(1000 / portTICK_RATE_MS);
    if (serial_data_str.empty()) {
      continue;
    }
    if (serial_data_str.compare("reset") == 0 || serial_data_str.compare("reboot") == 0) {
      std::cout << "\nRebooting...\n" << std::endl;
      xTaskCreate(&Puara::reboot_with_delay, "reboot_with_delay", 1024, NULL, 10, NULL);
    } else if (serial_data_str.compare("ping") == 0) {
      std::cout << "pong\n";
    } else if (serial_data_str.compare("whatareyou") == 0) {
      Puara::send_serial_data(Puara::dmiName);
    } else if (serial_data_str.rfind("sendconfig", 0) == 0) {
      serial_data_str_buffer = serial_data_str.substr(serial_data_str.find(" ") + 1);
      Puara::read_config_json_internal(serial_data_str_buffer);
    } else if (serial_data_str.rfind("writeconfig") == 0) {
      Puara::write_config_json();
    } else if (serial_data_str.compare("readconfig") == 0) {
      Puara::mount_spiffs();
      FILE* f = fopen("/spiffs/config.json", "r");
      if (f == NULL) {
        std::cout << "json: Failed to open file" << std::endl;
        return;
      }
      std::ifstream in("/spiffs/config.json");
      std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
      Puara::send_serial_data(contents);
      fclose(f);
      Puara::unmount_spiffs();
    } else if (serial_data_str.rfind("sendsettings", 0) == 0) {
      serial_data_str_buffer = serial_data_str.substr(serial_data_str.find(" ") + 1);
      Puara::read_settings_json_internal(serial_data_str_buffer, true);
    } else if (serial_data_str.rfind("writesettings") == 0) {
      Puara::write_settings_json();
    } else if (serial_data_str.compare("readsettings") == 0) {
      Puara::mount_spiffs();
      FILE* f = fopen("/spiffs/settings.json", "r");
      if (f == NULL) {
        std::cout << "json: Failed to open file" << std::endl;
        return;
      }
      std::ifstream in("/spiffs/settings.json");
      std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
      Puara::send_serial_data(contents);
      fclose(f);
      Puara::unmount_spiffs();
    } else {
      std::cout << "\nI don´t recognize the command \"" << serial_data_str << "\"" << std::endl;
    }
    serial_data_str.clear();
  }
}

/// REFACTOR: this is related to logging & debugging

void Puara::uart_monitor(void* pvParameters) {
  const int uart_num0 = 0;  // UART port 0
  uart_config_t uart_config0 = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,  // UART_HW_FLOWCTRL_CTS_RTS,
      .rx_flow_ctrl_thresh = 122,
      .source_clk = UART_SCLK_APB,
  };

  // Configure UART1 parameters
  uart_param_config(uart_num0, &uart_config0);

  uart_set_pin(
      uart_num0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  // Install UART driver (we don't need an event queue here)
  // In this example we don't even use a buffer for sending data.
  uart_driver_install(uart_num0, UART_FIFO_LEN + 1, 0, 0, NULL, 0);

  while (1) {
    // Read data from UART
    serial_data_length =
        uart_read_bytes(uart_num0, serial_data, PUARA_SERIAL_BUFSIZE, 500 / portTICK_RATE_MS);
    if (serial_data_length > 0) {
      serial_data_str = convertToString(serial_data);
      memset(serial_data, 0, sizeof serial_data);
      uart_flush(uart_num0);
    }
  }
}

void Puara::jtag_monitor(void* pvParameters) {
#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
  // Setup jtag module for USB Serial reads
  usb_serial_jtag_driver_config_t jtag_config{
      .tx_buffer_size = 256,
      .rx_buffer_size = 256,
  };

  // Install jtag module
  usb_serial_jtag_driver_install(&jtag_config);

  while (1) {
    // serial_data_length = USBSerial.read();
    // Only read if connected to PC
    serial_data_length =
        usb_serial_jtag_read_bytes(serial_data, PUARA_SERIAL_BUFSIZE, 500 / portTICK_RATE_MS);
    if (serial_data_length > 0) {
      serial_data_str = convertToString(serial_data);
      // remove new line character at end
      if (serial_data_str[serial_data_str.size() - 1] == '\n')
        serial_data_str.erase(serial_data_str.size() - 1);
      if (serial_data_str[serial_data_str.size() - 1] == '\r')
        serial_data_str.erase(serial_data_str.size() - 1);
      memset(serial_data, 0, sizeof serial_data);
    }
  }
#endif
}

void Puara::usb_monitor(void* pvParameters) {
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
  std::cout << "USB OTG monitor not supported, use the USB Serial JTAG or UART interface"
            << std::endl;
#endif
}

bool Puara::start_serial_listening() {
  // std::cout << "starting serial monitor \n";
  if (module_monitor == UART_MONITOR) {
    xTaskCreate(uart_monitor, "serial_monitor", 2048, NULL, 10, NULL);
    xTaskCreate(interpret_serial, "interpret_serial", 4096, NULL, 10, NULL);
  } else if (module_monitor == JTAG_MONITOR) {
    xTaskCreate(jtag_monitor, "serial_monitor", 2048, NULL, 10, NULL);
    xTaskCreate(interpret_serial, "interpret_serial", 4096, NULL, 10, NULL);
  } else if (module_monitor == USB_MONITOR) {
    xTaskCreate(usb_monitor, "serial_monitor", 2048, NULL, 10, NULL);
    xTaskCreate(interpret_serial, "interpret_serial", 4096, NULL, 10, NULL);
  } else {
    std::cout << "Invalid Monitor Type" << std::endl;
  }
  return 1;
}