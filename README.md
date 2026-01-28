# Puara Module

[![Build](https://github.com/Puara/puara-module/actions/workflows/build.yml/badge.svg)](https://github.com/Puara/puara-module/actions/workflows/build.yml)

This repository contains the source code for Puara Module library which offers the fundamentals to create ESP32-based devices that can be controlled over the network.
Why would I use this / Example projects : For artists wanting to create distibute systems, interactive systems, network based installations, new instrument/digital interfaces, etc..

## Overview

Puara Module facilitates embedded system development by providing a set of pre-defined modules that manage filesystem, web server, and network connections so users can focus on prototyping the rest of their system.

### PlatformIO 
Refer to the [PlatformIO templates and documentation](https://github.com/Puara/puara-module-template) to start prototyping with the Puara Module.

### Arduino 2.0 IDE
Download the `puara-module` library directly from the Arduino 2.0 IDE library manager to start prototyping with Puara Module in Arduino 2.0 IDE. 
Refer to the [Puara-Arduino repository](https://github.com/Puara/puara-arduino) for extra documentation and .ino examples.

## Tested boards and known issues

Generally, try to avoid esp32s3. We have seen the most issues with them.

### M5StickC
All good

### tinypico
All good

### Adafruit esp32s3 feather and TFT variant
- You will have to press the reset button while holding the boot button to be able to flash another firmware after flashing puara-module for the first time
- It can take a long time to get serial messages to print on the serial monitor.

### seeed xiao s3
- You will have to press the reset button while holding the boot button to be able to flash another firmware after flashing puara-module for the first time
- It can take a long time to get serial messages to print on the serial monitor.

These issues seem to be less frequent then on the esp32 feathers.


## More Info on the related [GuitarAMI](https://github.com/Puara/GuitarAMI) and [Puara](https://github.com/Puara) research

## How It Works

N.B. : Every template related to Puara Module has a different set of options, but all generally respct the following explanation.


When initiating the program, the module manager will try to connect to the WiFi Network (SSID) defined in `config.json`. 

If you want the board to connect to a specific WiFi network, modify the `wifiSSID` and `wifiPSK` values in `config.json` with your network name and password respectively and then build/upload the filesystem. 

After the board connects to an external SSID, it will also create its own WiFi Access Point **(STA-AP mode)**. 

If the process cannot connect to a valid SSID, it will still create its own WiFi Access Point **(AP mode)** to which users may connect and communicate with the board.

User may modify/add custom values in `settings.json` and access them in their program at any moment by using the **puara.getVarText("name")** and/or **puara.getVarNumber("name")** for text or number fields respectively; make sure to respect the JSON *name/value* pairing. 

User may modify said values via the web server settings page and the defined values will persist even after shutting down/rebooting the system. 
This is very useful if you wish to have easily configurable variables without having to rebuild/reflash your entire system.

To access the web server, connect to the same network/SSID as the board is connected to, or connect to the board's WiFi access point, and enter the board's IP address in any web browser. 

User may also type the network name followed by `.local` in the browser's address bar. Default network name is `device`_`id` (see `config.json file`) : **Puara_001**. Hence type `puara_001.local` in the browser's address bar to access web server pages.


## More Info on the research related with [Puara](https://github.com/Puara)

- [SAT](http://www.sat.qc.ca)
- [IDMIL](http://www.idmil.org)

## Licensing

The code in this project is licensed under the MIT license, unless otherwise specified within the file.
