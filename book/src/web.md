# Web Server

The `puara-module` library includes an integrated webserver that allows you to configure your device from a web browser. 
For more details on browser pages from a user perspective, see [Browser Pages](browser.md).

## Starting the webserver

The webserver is started automatically when you call the `puara.start()` method. You can also start and stop it manually using the `start_webserver()` and `stop_webserver()` methods:

```cpp
puara.start_webserver();
puara.stop_webserver();
```

When the webserver is running, you can access it by navigating to the IP address of your device in a web browser.

## Web pages

The web pages are stored in the `data` directory of your project. The `puara-module` library provides a set of default web pages that you can customize to your needs.

The default web pages are:

*   `index.html`: The main page of the web interface where user may find SSID configurations.
*   `scan.html`: Shows the available WiFi networks.
*   `settings.html`: Allows user to configure the custom variable values of the device. 
*   `update.html`: Currently unused. Contains legacy code for `Over the Air` updates.
*   `reboot.html`: Landing page confirming the reboot process.
*   `saved.html`: Landing page confirming the values were saved. This triggers the `onSettingsChanged()` if   `puara.set_settings_changed_handler(onSettingsChanged);` is defined in the setup routine.

You can edit these files to change the appearance and functionality of the web interface. The web pages use a simple template system that allows you to insert values from your code into the HTML. 