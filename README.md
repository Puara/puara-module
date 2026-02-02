# Puara Module

[![Build](https://github.com/Puara/puara-module/actions/workflows/build.yml/badge.svg)](https://github.com/Puara/puara-module/actions/workflows/build.yml)

This repository contains the source code for Puara Module library which offers the fundamentals to create ESP32-based devices that can be controlled over the network.

---

## Table of Contents
- [Why Use This?](#why-use-this)
- [Overview](#overview)
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

## Overview

Puara Module facilitates embedded system development by providing a set of pre-defined modules that manage filesystem, web server, and network connections so users can focus on prototyping the rest of their system. 

---

## Prototyping Options

Choose one of the following methods to start prototyping with the Puara Module :

### Option 1 : PlatformIO 
Refer to the [PlatformIO templates and documentation](https://github.com/Puara/puara-module-template) to start prototyping with PlatformIO.

### Option 2 : Arduino 2.0 IDE
Download the `puara-module` library directly from the Arduino 2.0 IDE library manager to start prototyping with Puara Module in Arduino 2.0 IDE. 
Find more information about this approach in the [Arduino examples and documentation](https://github.com/Puara/puara-arduino) .

---

## How It Works

 ⚠️ **Note:** Every template related to Puara Module has a different set of options but they all generally respect the following explanation.

### Connecting to WiFi

When initiating the program, the module manager will try to connect to the WiFi Network (SSID) defined in `config.json`. 

To connect to a specific WiFi network: 
- modify the `wifiSSID` and `wifiPSK` values in `config.json` with your network name and password
- build/upload the filesystem. 

After connecting to an external SSID: 
- The board will create its own WiFi Access Point **(STA-AP mode)**. 

If the board cannot connect to a valid SSID:
- It will still create its own WiFi Access Point **(AP mode)** for user connection

### Modifying Settings

Users can : 
- Modify/add custom values in `settings.json`
- Access these values in their program by using:
  - `puara.getVarText("name")` for text fields.
  - `puara.getVarNumber("name")` for number fields.
- Modify values via the web server settings page, with changes persisting after reboot.

### Accessing the Web Server

To access the web server:
- Connect to the same network/SSID as the board or connect to the board's WiFi access point. 
- Enter the board's IP address in any web browser. 
- Alternatively, type the network name followed by `.local` in the browser's address bar. Default network name is `device`_`id` (see `config.json file`) : **Puara_001**. Hence type `puara_001.local` in the browser's address bar to access web server pages.

--- 

## Tested Boards and Known Issues

| ✅ | Board                          | Notes                                                                                           | PlatformIO board ID | Arduino 2.0 IDE board ID |
|----|--------------------------------|-------------------------------------------------------------------------------------------------|---------------------|------------|
| ✅ | M5StickC                       | All good                                                                                       | m5stick-c| M5StickC | 
| ✅ | tinypico                       | All good                                                                                       | tinypico | UM TinyPico |
| ⚠️ | Adafruit ESP32-S3 Feather     | Hold Boot button and press Reset to enter serial flash mode. <br> Serial messages may take a long time to appear on the serial monitor. | adafruit_feather_esp32s3| Adafruit Feather ESP32-S3 2MB PSRAM | 
| ⚠️ | Adafruit ESP32-S3 TFT Feather | Hold Boot button and press Reset to enter serial flash mode. <br> Serial messages may take a long time to appear on the serial monitor. | adafruit_feather_esp32s3_tft| Adafruit Feather ESP32-S3 TFT|

| ⚠️ | Seeed Xiao S3                 | Hold Boot button and press Reset to enter serial flash mode. <br> Serial messages may take a long time to appear on the serial monitor. | seeed_xiao_esp32s3 | XIAO_ESP32S3 |

| ✅ | ESP32-C3-WROOM-02 // ESP32-C3-DevKitC-02  | All good                                      | esp32-c3-devkitc-02 | ESP32C3 Dev Module |

---

## References

Learn more about the research related to Puara: 

- [Puara GitHub Repository](https://github.com/Puara)
- [SAT](http://www.sat.qc.ca) // [SAT-R&D](https://sat.qc.ca/fr/recherche/)
- [IDMIL](http://www.idmil.org)

---

## Licensing

The code in this project is licensed under the MIT license, unless otherwise specified within the file.
