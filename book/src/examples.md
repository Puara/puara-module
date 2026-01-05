# Examples

This chapter provides a more detailed explanation of the examples included with the `puara-module` library.

## `puara_basic_template`

The `puara_basic_template` example shows how to use the basic functionalities of the `puara-module` library.

### `puara_basic_template.ino`

This is the main file of the example. It shows how to:

*   Initialize the `puara-module` library.
*   Set a callback function for when the settings are changed.
*   Get the values of variables from the `settings.json` file.
*   Send data over the serial port.

```cpp
#include <puara.h>
#include <iostream>
// Initialize Puara's module manager
Puara puara;

// Dummy sensor data
float sensor;

void onPuaraSettingsChanged()
{
  // Process here any change of settings by the user, OSC port, etc.
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);

    puara.set_settings_changed_handler(onPuaraSettingsChanged);
      /*
     * the Puara start function initializes the filesystem, reads json configuration and
     * settings, start the wi-fi AP or connects to SSID, starts the webserver, inits serial 
     * listening, MDNS service, and scans for WiFi networks.
     */
    puara.start();

    /* 
     * Printing custom settings stored. The data/config.json values will print during 
     * Initialization (puara.start)
     */
    Serial.println("\nSettings stored in data/settings.json:");

    Serial.print("Hitchhiker: ");
    Serial.println(puara.getVarText("Hitchhiker").c_str());

    Serial.print("answer_to_everything: ");
    Serial.println(puara.getVarNumber("answer_to_everything"));

    Serial.print("variable3: ");
    Serial.println(puara.getVarNumber("variable3")); 
}

void loop() {

  // put your main code here, to run repeatedly:
      // Update the dummy sensor variable with random number
    sensor = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10));
    
    // print the dummy sensor data
    Serial.println();
    Serial.print("Dummy sensor value: ");
    Serial.println(sensor);

    // run at 1 Hz (1 message per second)
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
```

### `data` directory

The `data` directory contains the `config.json` and `settings.json` files, as well as the web pages for the web interface. You can customize these files to your needs.

```