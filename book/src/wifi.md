# WiFi Considerations

The `puara-module` library provides a simple and flexible interface for managing WiFi connections. This document outlines the key WiFi-related features, modes, and public API functions available in the library.

## WiFi Modes 

The `puara-module` supports three modes of operation:

1. **Station - Access Point (STA-AP) Mode**:
   - The device connects to an existing WiFi network (station). 
   - The device creates its own WiFi network (access point).
   
2. **Access Point (AP) Mode**:
   - The device does not connect to an existing WiFi network. 
   - The device creates its own WiFi network (access point).

   - Useful for initial configuration or when no external WiFi network is available.

3. **Station (STA) Mode**:
   - The device connects to an existing WiFi network.
   - The Access Point is turned off with `persistent_AP=0`
   - Useful to limit Wifi pollution and securing device.

### Default Behavior
Device is STA-AP by default: 
  - The `puara-module` will attempt to connect to a known WiFi network. If the connection succeeds or fails, the device will then activate it's AP mode.

To deactivate STA, leave useless connection information (SSID, PSK) in `config.json`. 
To deactivate AP mode, set the `persistent_AP` option to `0` in the `config.json` file or access the config page through the web server and tick the proper option about AP mode.

---


## Public API Functions

The `puara-module` exposes the following public API functions for managing WiFi:

### `void start_wifi()`
- **Description**: Initializes the WiFi configuration and starts the WiFi service.
- **Behavior**:
  - Configures the device to operate in either STA-AP, STA or AP mode based on the `config.json` settings.
  - Ensures default values for SSID and passwords if they are missing or invalid.


  ### `void wifi_scan()`
- **Description**: Scans for available WiFi networks and stores the results.
- **Behavior**:
  - The scan results include SSID, RSSI (signal strength), and channel information for each network.
  - Results are displayed on the scan.html page of the web interface.

### `bool get_StaIsConnected()`
- **Description**: Checks whether the device is connected to an external WiFi network in STA mode.
- **Returns**: Returns true if the device is connected, false otherwise.

### `std::string staIP()`
- **Description**: Retrieves the current IP address of the device when connected to an external WiFi network in STA-IP or STA modes.

- **Returns**: The IP address as a string.