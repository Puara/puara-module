# Summary

`puara-module` is a library for the ESP32 boards that provides a simplified interface for managing WiFi, a webserver, mDNS, and a filesystem in order to facilitate the creation of distributed interactive systems, networked controllers and other similar applications. 

### Documentation
This documentation is divided into two main sections, one dedicated to users with a focus on compiling and uploading the templates and filesystem, and another focused on more technical details about the code aimed at helping developpers and curious users understand the library.

#### 1. Useful information for running a Puara Module projects
- [Installing Puara Module library and related dependencies](library_installation.md)
- [Configuring your board and network specifications](configuration.md)
- [Building and uploading the firwmare and filesystem](examples.md)
- [Using the browser to modify settings](browser.md)

> ##### **Important detail for users**
> Most Arduino or embedded projects only upload the **code** that runs on the device. However, in this project, the device also needs a **filesystem** to store important data, such as configuration files, templates, or other resources that the code relies on. These two parts—**code** and **filesystem**—serve different purposes and must be built and uploaded separately.
> The **executable code** tells the device what to do, includes the logic, instructions, and behavior of the device such as how to read a sensor, process data, or send information over Wi-Fi.
> The **filesystem** is like a "hard drive" for the device, where additional files are stored and can include configuration files, templates, or other resources that the code needs to function properly. In our approach, the filesystem stores a JSON file with user settings for the network configurations and some global variables that can be modified through the browser without needing to reflash the whole system.
> 

#### 2. Useful information for developpers 
- [The `Puara` object](puara.md) : Use of the `Puara` object to manage your device.
- [Filesystem](filesystem.md) : Configuration differences between SPIFFS and LITTLEFS. 
- [Webserver](web.md) : Use the integrated webserver.
- [WiFi](wifi.md) : Manage WiFi connections.
- [Serial](serial.md) : Communicate over serial.
- [mDNS](mdns.md) :  Use of the mDNS service.


---