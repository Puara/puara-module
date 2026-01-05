# The `Puara` object

The `Puara` object is the main entry point for using the `puara-module` library. It provides a simple interface for managing the different modules of the library.

## Initialization

To use the `Puara` object, you first need to include the `puara.h` header file and create an instance of the `Puara` class:

```cpp
#include <puara.h>

Puara puara;
```

Then, in your `setup()` function, you need to call the `start()` method:

```cpp
void setup() {
    puara.start();
}
```

The `start()` method initializes the filesystem, reads the configuration and settings from the JSON files, starts the WiFi in AP or STA mode, starts the webserver, initializes serial listening, starts the mDNS service, and scans for WiFi networks.

## Methods

The `Puara` object has several methods for interacting with the different modules of the library. Here are some of the most important ones:

*   `start(PuaraAPI::Monitors monitor = PuaraAPI::UART_MONITOR)`: Initializes the `puara-module`. You can specify the monitor to be used for logging. The default is `UART_MONITOR`.
*   `start_webserver()`: Starts the webserver.
*   `stop_webserver()`: Stops the webserver.
*   `dmi_name()`: Returns the DMI name of the device.
*   `version()`: Returns the version of the `puara-module` library.
*   `set_version(unsigned int user_version)`: Sets the version of your application.
*   `IP1()`: Returns the primary IP address.
*   `IP2()`: Returns the secondary IP address.
*   `set_settings_changed_handler(std::function<void()>)`: Sets a callback function that will be called when the settings are changed by the user.
*   `getVarNumber(std::string varName)`: Returns the value of a number variable from the `settings.json` file.
*   `getVarText(std::string varName)`: Returns the value of a text variable from the `settings.json` file.

In the next chapters, we will explore the different modules of the `puara-module` library in more detail.
