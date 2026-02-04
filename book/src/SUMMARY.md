# Summary

---

**Société des Arts Technologiques (SAT)**  
**Input Devices and Music Interaction Laboratory (IDMIL)**

--- 

This documentation covers the applied usage of the Puara Module library and details the various modules found in the current repository.

## Overview

Puara Module is a library for the ESP32 boards that provides a simplified interface for managing WiFi, a web server, and a filesystem in order to facilitate the creation of distributed interactive systems, networked controllers and other similar applications so users can focus on prototyping the rest of their system.

## Documentation
This documentation is divided into two main sections:
- Building and compiling Puara Module templates/examples:
  - Using the Arduino 2.0 IDE examples;
  - Using the PlatformIO templates in VS Code;
- Technical details about the code aimed at helping developpers understand the codebase.


## 1. Building Puara Module Templates/Examples

Please choose your preferred development environment. We offer almost identical templates in both Arduino 2.0 IDE as in the PlatformIO extension to VS Code.

For beginner to intermediate level programmers, we recommend using the Arduino approach. As for intermediate to advanced programmers, or users wishing to have a more granular capacity with their projects, we recommend the PlatformIO - VS Code IDE approach.
- [Arduino User](arduino_user.md)
- [PlatformIO User](pio_user.md)

Using the web server to modify your network configurations and program variables in settings:
- [Browser Pages](browser.md)


#### 2. Useful information for developpers 
- [`Puara` object and API](puara.md) : Use of the `Puara` object to manage your device.
- [Filesystem](filesystem.md) : Configuration differences between SPIFFS and LITTLEFS. 
- [Webserver](web.md) : Use the integrated webserver.
 - See [Browser Pages](browser.md) for user-facing web server considerations.
- [WiFi](wifi.md) : Manage WiFi connections.
- [Serial and Debug Printing](serial.md) : Communicate over serial and printing puara debug messages.
- [mDNS](mdns.md) :  Use of the mDNS service.


---