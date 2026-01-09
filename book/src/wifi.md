# WiFi

The `puara-module` library provides a simple interface for managing WiFi connections.

## AP and STA modes

The `puara-module` can work in two modes:

*   **Access Point (AP) mode**: The device creates its own WiFi network. This is useful for initial configuration.
*   **Station (STA) mode**: The device connects to an existing WiFi network.

By default, the `puara-module` will start in AP mode if it cannot connect to a known WiFi network. You can force the device to always start in AP mode by setting the `persistent_AP` option to `true` in the `config.json` file.

## WiFi scan

You can scan for available WiFi networks using the `wifi_scan()` method:

```cpp
puara.wifi_scan();
```

The results of the scan will be shown in the `scan.html` page of the web interface.

## IP addresses

You can get the IP addresses of the device using the `IP1()` and `IP2()` methods:

```cpp
std::string ip1 = puara.IP1();
std::string ip2 = puara.IP2();
```

The `IP1()` method returns the IP address of the STA interface, while the `IP2()` method returns the IP address of the AP interface.
