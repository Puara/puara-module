# Puara API Documentation

The `Puara` object is the main entry point for using the `puara-module` library. It provides a simple interface for managing the different modules of the library, including WiFi, filesystem, web server, and more.

---


## Initialization

To use the `Puara` object, you first need to include the `puara.h` header file and create an instance of the `Puara` class:

```cpp
#include <puara.h>

Puara puara;
```

Then, in your `setup()` function, call the `start()` method:

```cpp
void setup() {
    puara.start();
}
```

The `start()` method initializes the following:
*   Filesystem
*   Configuration and settings (from the JSON files)
*   WiFi (in STA-AP, AP or STA mode)
*   Web server
*   Serial listening
*   mDNS service
*   WiFi network scanning

## Public Methods

### Initialization & Core

#### `start()`

```cpp
void start(PuaraAPI::Monitors monitor = PuaraAPI::UART_MONITOR, esp_log_level_t debug_level = ESP_LOG_WARN);
```

Initializes the Puara module and all its subsystems. This is the main entry point and should be called in your `setup()` function.

**Parameters:**
- `monitor`: Specifies the serial monitor type. Options are:
  - `PuaraAPI::UART_MONITOR` (default) - Standard UART serial monitor
  - `PuaraAPI::JTAG_MONITOR` - JTAG-based debugging monitor
  - `PuaraAPI::USB_MONITOR` - USB serial monitor (not currently functional)
- `debug_level`: Sets the ESP-IDF logging level. Default is `ESP_LOG_WARN`. Other options include `ESP_LOG_NONE`, `ESP_LOG_ERROR`, `ESP_LOG_INFO`, `ESP_LOG_DEBUG`, `ESP_LOG_VERBOSE`.

---

### Web Server

#### `start_webserver()`

```cpp
httpd_handle_t start_webserver(void);
```

Starts the built-in HTTP web server for device configuration and monitoring.

**Returns:** A handle to the HTTP server instance (`httpd_handle_t`).

#### `stop_webserver()`

```cpp
void stop_webserver(void);
```

Stops the HTTP web server.

---

### Device Information

#### `dmi_name()`

```cpp
std::string dmi_name();
```

Returns the DMI (Device/Module Identifier) name of the device, which is a combination of the device type and ID.

**Returns:** The device's DMI name as a `std::string`.

#### `version()`

```cpp
unsigned int version();
```

Returns the current firmware version number.

**Returns:** The version as an `unsigned int`.

#### `set_version()`

```cpp
void set_version(unsigned int user_version);
```

Sets a custom firmware version number.

**Parameters:**
- `user_version`: The version number to set.

---

### Filesystem

#### `mount()`

```cpp
void mount();
```

Mounts the device's filesystem (LittleFS or SPIFFS). This is automatically called by `start()`, but can be used manually if needed.

#### `unmount()`

```cpp
void unmount();
```

Unmounts the device's filesystem. Useful before deep sleep or when filesystem access is no longer needed.

---

### Configuration Management

#### `read_config_json()`

```cpp
void read_config_json();
```

Reads the device configuration from the `config.json` file stored in the filesystem. This includes device name, ID, author, institution, and WiFi credentials.

#### `write_config_json()`

```cpp
void write_config_json();
```

Writes the current device configuration to the `config.json` file in the filesystem.

#### `read_settings_json()`

```cpp
void read_settings_json();
```

Reads user-defined settings from the `settings.json` file stored in the filesystem.

#### `write_settings_json()`

```cpp
void write_settings_json();
```

Writes the current user-defined settings to the `settings.json` file in the filesystem.

#### `set_settings_changed_handler()`

```cpp
void set_settings_changed_handler(std::function<void()>);
```

Registers a callback function that will be called whenever settings are modified (e.g., through the web interface).

**Parameters:**
- A `std::function<void()>` callback that takes no arguments and returns nothing.

**Example:**
```cpp
puara.set_settings_changed_handler([]() {
    Serial.println("Settings have been updated!");
    // Reload or apply new settings here
});
```

---

### Serial Communication

#### `start_serial_listening()`

```cpp
bool start_serial_listening();
```

Starts listening for serial commands. This allows configuration and control via serial terminal.

**Returns:** `true` if serial listening was successfully started, `false` otherwise.

#### `send_serial_data()`

```cpp
void send_serial_data(std::string data);
```

Sends data through the serial interface.

**Parameters:**
- `data`: The string data to send.

---

### mDNS Service

#### `start_mdns_service()`

```cpp
void start_mdns_service(std::string_view device_name, std::string_view instance_name);
```

Starts the mDNS (multicast DNS) service, allowing the device to be discovered on the local network by its hostname.

**Parameters:**
- `device_name`: The hostname for mDNS discovery (e.g., "puara-device").
- `instance_name`: A human-readable instance name for the service.

---

### WiFi Management

#### `start_wifi()`

```cpp
void start_wifi();
```

Starts the WiFi subsystem based on the configuration. The device can operate in:
- **STA mode**: Connects to an existing WiFi network
- **AP mode**: Creates its own access point
- **STA-AP mode**: Both simultaneously

#### `wifi_scan()`

```cpp
void wifi_scan(void);
```

Initiates a scan for available WiFi networks. Results can be viewed through the web interface.

#### `get_StaIsConnected()`

```cpp
bool get_StaIsConnected();
```

Checks if the device is connected to a WiFi network in station (STA) mode.

**Returns:** `true` if connected to a WiFi access point, `false` otherwise.

#### `staIP()`

```cpp
std::string staIP();
```

Returns the device's IP address when connected to an external WiFi access point.

**Returns:** The IP address as a `std::string` (e.g., "192.168.1.100").

---

### Variable Access

#### `getVarNumber()`

```cpp
double getVarNumber(std::string varName);
```

Retrieves the value of a numeric setting variable by name from the settings.

**Parameters:**
- `varName`: The name of the variable to retrieve.

**Returns:** The value of the variable as a `double`.

#### `getVarText()`

```cpp
std::string getVarText(std::string varName);
```

Retrieves the value of a text setting variable by name from the settings.

**Parameters:**
- `varName`: The name of the variable to retrieve.

**Returns:** The value of the variable as a `std::string`.

