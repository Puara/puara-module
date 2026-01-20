# Puara Module

Library for creating music controllers using the Puara framework

It includes a Wi-Fi manager (set SSID/password for wireless networks), a [OSC](https://en.wikipedia.org/wiki/Open_Sound_Control) manager (set addresses to send OSC messages), setup tools, and [libmapper](http://libmapper.github.io/) compatibility*.

This pseudo-library currently has the following dependencies:

- [libmapper-arduino](https://github.com/puara/libmapper-arduino.git) (cloned from the [original library](https://github.com/mathiasbredholt/libmapper-arduino)).

## How to use

Use the [template](https://github.com/Puara/puara-module-template) to start using the Puara Module.

## Known Issues
- With esp32s3 boards, after flashing puara-module for the first time it is often necessary to hold the boot button and to press reset to be able to flash anything else.

- Still with esp32s3 boards, serial monitors typically do not work.

## More Info on the related [GuitarAMI](https://github.com/Puara/GuitarAMI) and [Puara](https://github.com/Puara) research

[https://www.edumeneses.com](https://www.edumeneses.com)

[http://www-new.idmil.org/project/guitarami/](http://www-new.idmil.org/project/guitarami/)

## Licensing

The code in this project is licensed under the MIT license, unless otherwise specified within the file.
