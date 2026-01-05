#include <puara.h>
#include <puara-application-settings.hpp>
#include <format>
#include <iostream>
#include <reflect>

// Initialize Puara's module manager
Puara puara;

// Dummy sensor data
float sensor;

void onPuaraSettingsChanged()
{
  // Process here any change of settings by the user, OSC port, etc.
}

void setup()
{
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

void loop()
{

  // put your main code here, to run repeatedly:
  // Update the dummy sensor variable with random number
  sensor = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 10));

  // print the dummy sensor data
  Serial.println();
  Serial.print("Dummy sensor value: ");
  Serial.println(sensor);
  Serial.print("Dummy settings value: ");
  Serial.println(settings.port);


  // run at 1 Hz (1 message per second)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
