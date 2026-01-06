# Summary

### About
`puara-module` is a library for the ESP32 boards that provides a simplified interface for managing WiFi, a webserver, mDNS, and a filesystem in order to facilitate the creation of networked controllers, musical interfaces, distributed interactive systems, and other similar applications. 

### Documentation
This documentation is divided into two sections, one dedicated to users with a focus on compiling and uploading the templates, and another focused on details about the code aimed at helping developpers understand the library. 

#### Useful information for running Puara Module projects
- [Installing Library](library_installation.md) : Install the `puara-module` library and it's related dependencies.
- [Configuration](configuration.md) : Configure your board and network specifications.
- [Examples](examples.md) : Build and upload the firmware and filesystem.

#### Useful information for developpers 
- [The `Puara` object](puara.md) : Use of the `Puara` object to manage your device.
- [Filesystem](filesystem.md) : Differences between SPIFFS and LITTLEFS 
- [Webserver](web.md) : Use the integrated webserver.
- [WiFi](wifi.md) : Manage WiFi connections.
- [Serial](serial.md) : Communicate over serial.
- [mDNS](mdns.md) :  Use of the mDNS service.


### Important detail about Code and Filesystem approach

Most Arduino or embedded projects only upload the **code** that runs on the device. However, in this project, the device also needs a **filesystem** to store important data, such as configuration files, templates, or other resources that the code relies on. These two parts—**code** and **filesystem**—serve different purposes and must be built and uploaded separately.
The **executable code** tells the device what to do, includes the logic, instructions, and behavior of the device such as how to read a sensor, process data, or send information over Wi-Fi.
The **filesystem** is like a "hard drive" for the device, where additional files are stored and can include configuration files, templates, or other resources that the code needs to function properly. In our approach, the filesystem stores a JSON file with user settings for the network configurations.

---