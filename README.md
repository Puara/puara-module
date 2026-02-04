# Puara Module

---

**Société des Arts Technologiques (SAT)**  
**Input Devices and Music Interaction Laboratory (IDMIL)**

--- 

This repository contains the Puara Module source code used to facilitate the creation of connected devices. 
For applid usage, see either the [PlatformIO templates](https://github.com/Puara/puara-module-templates) or the [Arduino examples](https://github.com/Puara/puara-arduino).

--- 

## Overview

Puara Module is a library for the ESP32 boards that provides a simplified interface for managing WiFi, a web server, and a filesystem in order to facilitate the creation of distributed interactive systems, networked controllers and other similar applications so users can focus on prototyping the rest of their system. 
 
---

## Table of Contents
- [Why Use This?](#why-use-this)
- [Prototyping Options](#prototyping-options)
- [How It Works](#how-it-works)
- [Tested Boards and Known Issues](#tested-boards-and-known-issues)
- [References](#references)
- [Licensing](#licensing)

---

## Why Use This?

This project is designed for artists and creators interested in:
- Developing distributed systems
- Creating interactive installations
- Building network-based projects
- Innovating digital instruments
- Designing new forms of creative interfaces

---

## Prototyping Options

Choose one of the following methods to start prototyping with the Puara Module :
- For beginners and intermediate level developpers, refer to [Arduino examples and documentation](https://github.com/Puara/puara-arduino) to start prototyping with Arduino 2.0 IDE.

- For intermediate and advanced developpers, refer to the [PlatformIO templates](https://github.com/Puara/puara-module-template) to start prototyping with PlatformIO and VS Code.

---

## How It Works
## How It Works

**Every template related to Puara Module has a different set of options but they all generally respect the following explanation.**
The following sections are detailed more thoroughly in the [Puara Module](https://github.com/Puara/puara-module) doumentation.

### 1. Establishing WiFi 

When initiating the program, the module manager will try to connect to the WiFi Network (SSID) defined in `config.json`. 
The `puara-module` supports three modes of operation:

1. **Station - Access Point (STA-AP) Mode** (Default):
   - The device connects to an existing WiFi network (station). 
   - The device creates its own WiFi network (access point).
   - User has two ways to communicate with the board.
   
2. **Access Point (AP) Mode**:
   - The device does not connect to an existing WiFi network. 
   - The device creates its own WiFi network (access point).

3. **Station (STA) Mode**:
   - The device connects to an existing WiFi network.
   - The Access Point is turned off with `persistent_AP=0`
   - Useful to limit Wifi pollution and securing device.


### 2. Making the Web Server Accessible

Browser-accessible pages available for configuring, scanning, and managing settings on your device are made availabe through Puara Module.

Once the web server is running, you can access it in two ways:

1. **Via IP Address**: Navigate to the device's IP address in your web browser (e.g., `http://192.168.4.1` for AP mode, or the assigned IP in STA/STA-AP modes which can be retrieved using `puara.staIP()`).

2. **Via mDNS Hostname**: If mDNS is enabled, you can access the device using its hostname (e.g., `http://your-device-name.local`). Default `config.json`values enable mDNS with the Puara_001, and browser is accessible with `puara_001.local`. 

Using the web browser, users can modify variables that keep their value after reboot/shutdown of device without needing to rebuild/upload their program.
Access these values in the program by using:

```cpp
// For text fields
std::string my_string = puara.getVarText("variable_name");

// For number fields (all numbers are `doubles` -- see JSON documentation for explanation)
double my_number = puara.getVarNumber("variable_name");
```

#### Making of Custom Variables in `settings.json`

The `/data/settings.json` file stores custom application settings as an array of name-value pairs:

```json
{
    "settings": [
        {
            "name": "user_defined_text",
            "value": "user defined value"
        },
        {
            "name": "variable3",
            "value": 12.345
        }
    ]
}
```
User may add/modify fields in this file and then upload the new filesystem in order to have a more custom device.

--- 

For more detailed documentation, please refer to the mdBook in the puara-module's github repository book.

--- 

## Tested Boards and Known Issues

| ✅ | Board                          | Notes                                                                                           | PlatformIO board ID | Arduino 2.0 IDE board ID |
|----|--------------------------------|-------------------------------------------------------------------------------------------------|---------------------|------------|
| ✅ | M5StickC                       | All good                                                                                       | m5stick-c| M5StickC | 
| ✅ | tinypico                       | All good                                                                                       | tinypico | UM TinyPico |
| ✅ | ESP32-C3-WROOM-02 // ESP32-C3-DevKitC-02  | All good                                      | esp32-c3-devkitc-02 | ESP32C3 Dev Module |
| ⚠️ | Adafruit ESP32-S3 Feather     | Hold Boot button and press Reset to enter serial flash mode. <br> Serial messages may take a long time to appear on the serial monitor. | adafruit_feather_esp32s3| Adafruit Feather ESP32-S3 2MB PSRAM | 
| ⚠️ | Adafruit ESP32-S3 TFT Feather | Hold Boot button and press Reset to enter serial flash mode. <br> Serial messages may take a long time to appear on the serial monitor. | adafruit_feather_esp32s3_tft| Adafruit Feather ESP32-S3 TFT|
| ⚠️ | Seeed Xiao S3                 | Hold Boot button and press Reset to enter serial flash mode. <br> Serial messages may take a long time to appear on the serial monitor. | seeed_xiao_esp32s3 | XIAO_ESP32S3 |
| ⚠️ | DOIT ESP32 DevKit V1  | Arduino IDE : Tools/Partition Scheme options simply unavailable hence build impossible. <br> Function with PlatformIO however    | esp32doit-devkit-v1 | DOIT ESP32 DEVKIT V1 |

---

## References

Learn more about the research related to Puara: 

- [Puara GitHub Repository](https://github.com/Puara)
- [SAT](http://www.sat.qc.ca) // [SAT-R&D](https://sat.qc.ca/fr/recherche/)
- [IDMIL](http://www.idmil.org)

---

## Licensing

The code in this project is licensed under the MIT license, unless otherwise specified within the file.
