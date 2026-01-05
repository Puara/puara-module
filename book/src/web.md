# Webserver

The `puara-module` library includes an integrated webserver that allows you to configure your device from a web browser.

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

*   `index.html`: The main page of the web interface.
*   `scan.html`: Shows the available WiFi networks.
*   `settings.html`: Allows you to configure the settings of your device.
*   `update.html`: Allows you to update the firmware of your device.
*   `factory.html`: Allows you to reset your device to factory settings.
*   `reboot.html`: Allows you to reboot your device.

You can edit these files to change the appearance and functionality of the web interface. The web pages use a simple template system that allows you to insert values from your code into the HTML. For example, you can use `{{dmi_name}}` to insert the DMI name of your device.
