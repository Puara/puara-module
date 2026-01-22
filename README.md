# Puara Module

Library for creating music controllers using the Puara framework

It includes a Wi-Fi manager (set SSID/password for wireless networks), a [OSC](https://en.wikipedia.org/wiki/Open_Sound_Control) manager (set addresses to send OSC messages), setup tools, and [libmapper](http://libmapper.github.io/) compatibility*.

This pseudo-library currently has the following dependencies:

- [libmapper-arduino](https://github.com/puara/libmapper-arduino.git) (cloned from the [original library](https://github.com/mathiasbredholt/libmapper-arduino)).

## How to use

Use the [template](https://github.com/Puara/puara-module-template) to start using the Puara Module.

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

[https://www.edumeneses.com](https://www.edumeneses.com)

[http://www-new.idmil.org/project/guitarami/](http://www-new.idmil.org/project/guitarami/)

## Licensing

The code in this project is licensed under the MIT license, unless otherwise specified within the file.
