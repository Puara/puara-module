# Puara Module for Arduino

## Dependencies

 ⚠️ Download [Arduino 2.0 IDE](https://www.arduino.cc/en/software/) for build/upload of program.

 ⚠️ Install [Arduino-LittleFS-Upload](https://github.com/earlephilhower/arduino-littlefs-upload) for filesystem uploading.

 ⚠️ ESP32 board library : Open **Boards Manager** icon in Arduino 2.0 IDE, type **esp32** in the Boars Manager **search bar** and install *esp32* by *Espressif Systems*.

 ⚠️ Partition size options : Select any options that offer minimal partition before building program. These options vary depending on board capabilities and can be found here : `/Tools/Partition Scheme/`. A common option is `Minimal SPIFFS` such as  : `/Tools/Partition Scheme/Minimal SPIFFS`. 
 Warning, some boards simply do not have such options.

## How to Use

1. **Install Arduino 2.0 IDE**

2. **Dowload the `puara-module` library from library manager**

3. **Open a puara template in Arduino IDE**: Open Arduino IDE, go to File/Examples/puara-module/ and select the template of your choice. 

4. **Configure the board**: Ensure the `board` and `port` variables in the IDE match your board. 

5. **Edit partition scheme** : Allow more space for your programs. Go to :
- Tools/Partition Scheme/
Select `Minimal Spiffs` or similar option. Different boards have different possibilities but generally the `Minimal Spiffs:...` option should be present.

6. **Edit the template**: You are now ready to edit the template according to your board/needs.

7. **Edit the filesystem**: To modify network configurations, add or modify the available variables in the settings, got to :
- `Sketch/Show Sketch Folder` which will open your local project folder
- Enter `data/` folder:
  - Modify network name (SSID) and password (PSK) configurations in config.json;
  - Modify/Add variables in settings.json;
  - Save your modifications.

8. **Build and upload the filesystem and firmware**: Once ready, you can:
- Use Arduino IDE to build and upload the firmware to your board.
- Use [Arduino-LittleFS-Upload](https://github.com/earlephilhower/arduino-littlefs-upload) for the filesystem uploading:
`[Ctrl]` + `[Shift]` + `[P]`, then "`Upload LittleFS to Pico/ESP8266/ESP32`".

On macOS, press `[⌘]` + `[Shift]` + `[P]` to open the Command Palette in the Arduino IDE, then "`Upload LittleFS to Pico/ESP8266/ESP32`".

9. **Test your IoT device and validate communication between systems**: At this point your board should either connect to your specified network if it can find it. In either case, if it does or doesn't connect, your board will create an Access Point you can connect to directly. Use the board's IP address to communicate with it when needed. More details below.

> #####  ⚠️ **Important detail for users**
> Most Arduino or embedded projects only upload the **code** that runs on the device. However, in this project, the device also needs a **filesystem** to store important data, such as configuration files, templates, or other resources that the code relies on. These two parts—**code** and **filesystem**—serve different purposes and must be built and uploaded separately.
> The **executable code** tells the device what to do, includes the logic, instructions, and behavior of the device such as how to read a sensor, process data, or send information over Wi-Fi.
> The **filesystem** is like a "hard drive" for the device, where additional files are stored and can include configuration files, templates, or other resources that the code needs to function properly. In our approach, the filesystem stores a JSON file with user settings for the network configurations and some global variables that can be modified through the browser without needing to reflash the whole system.
> 

---

## How It Works

 ⚠️ 
**Every template related to Puara Module has a different set of options but they all generally respect the following explanation.**
The following sections are detailed more thoroughly in the [Puara Module](https://github.com/Puara/puara-module) doumentation.

### 1. Establishing WiFi 

When initiating the program, the module manager will try to connect to the WiFi Network (SSID) defined in `config.json`. 
The `puara-module` supports three modes of operation:

1. **Station - Access Point (STA-AP) Mode** (Default):
   - The device connects to an existing WiFi network (station). 
   - The device creates its own WiFi network (access point).
   - User has two ways to communicate with the board.
   
2. **Access Point (AP) Mode**:
   - The device does not connect to an existing WiFi network. 
   - The device creates its own WiFi network (access point).

3. **Station (STA) Mode**:
   - The device connects to an existing WiFi network.
   - The Access Point is turned off with `persistent_AP=0`
   - Useful to limit Wifi pollution and securing device.


### 2. Making the Web Server Accessible

Browser-accessible pages available for configuring, scanning, and managing settings on your device are made availabe through Puara Module.

Once the web server is running, you can access it in two ways:

1. **Via IP Address**: Navigate to the device's IP address in your web browser (e.g., `http://192.168.4.1` for AP mode, or the assigned IP in STA/STA-AP modes which can be retrieved using `puara.staIP()`).

2. **Via mDNS Hostname**: If mDNS is enabled, you can access the device using its hostname (e.g., `http://your-device-name.local`). Default `config.json`values enable mDNS with the Puara_001, and browser is accessible with `puara_001.local`. 

Using the web browser, users can modify variables that keep their value after reboot/shutdown of device without needing to rebuild/upload their program.
Access these values in the program by using:

```cpp
// For text fields
std::string my_string = puara.getVarText("variable_name");

// For number fields (all numbers are `doubles` -- see JSON documentation for explanation)
double my_number = puara.getVarNumber("variable_name");
```

#### Making of Custom Variables in `settings.json`

The `/data/settings.json` file stores custom application settings as an array of name-value pairs:

```json
{
    "settings": [
        {
            "name": "user_defined_text",
            "value": "user defined value"
        },
        {
            "name": "variable3",
            "value": 12.345
        }
    ]
}
```
User may add/modify fields in this file and then upload the new filesystem in order to have a more custom device.

--- 

For more detailed documentation, please refer to the mdBook in the puara-module's github repository.

--- 


## Examples

The [puara-arduino](https://github.com/Puara/puara-arduino) repository includes five Arduino sketches that demonstrate different use cases and functionalities. 
All examples are identical to the PlatformIO templates available in [puara-module-templates](https://github.com/Puara/puara-module-templates).

Each example includes a `data/` folder containing configuration files (`config.json`, `settings.json`) and web interface files (HTML and CSS). 

 ⚠️ **After building and uploading the firmware to your board, you must also upload the filesystem**.


### 1. Basic Example

**File**: `examples/basic/basic.ino`

A minimal example demonstrating core Puara Module functionality. This example:
- Initializes the Puara module manager
- Reads custom settings from the JSON configuration files
- Outputs dummy sensor data to the serial monitor
- Demonstrates how to access custom configuration variables using `getVarText()` and `getVarNumber()`

This is the best starting point for learning how to use the Puara framework.

### 2. OSC-Send Example

**File**: `examples/OSC-Send/OSC-Send.ino`

Demonstrates how to set up a basic OSC transmitter. This example:
- Sends dummy sensor data as OSC messages to a specified IP address and port
- Configurable OSC IP/port via the web interface without rebuilding/reflashing
- Shows how to use the `onSettingsChanged()` callback to update parameters dynamically
- Includes example code for reading analog sensors and digital signals

**Note**: Please refer to [CNMAT's OSC repository](https://github.com/CNMAT/OSC) on GitHub for more details on OSC.

### 3. OSC-Receive Example

**File**: `examples/OSC-Receive/OSC-Receive.ino`

Demonstrates how to receive and process OSC messages. This example:
- Listens for incoming OSC messages on a configurable UDP port
- Parses OSC messages and extracts data from them
- Demonstrates example processing of float values to control device outputs (e.g., LED brightness)
- Shows how to use the `onSettingsChanged()` callback for dynamic configuration

The example expects a float between [0,1] on the OSC address `/led/brightness` with the format: `/led/brightness f 0.34`

**Note**: Please refer to [CNMAT's OSC repository](https://github.com/CNMAT/OSC) on GitHub for more details on OSC.

### 4. OSC-Duplex Example

**File**: `examples/OSC-Duplex/OSC-Duplex.ino`

Combines both OSC-Send and OSC-Receive functionality in a single sketch. This example:
- Sends dummy sensor data to a remote OSC address
- Simultaneously receives OSC messages from remote sources
- Demonstrates full duplex OSC communication patterns
- Useful for bidirectional device communication scenarios

**Note**: Please refer to [CNMAT's OSC repository](https://github.com/CNMAT/OSC) on GitHub for more details on OSC.

### 5. BLE Advertising Example

**File**: `examples/ble-advertising/ble-advertising.ino`

Demonstrates BLE (Bluetooth Low Energy) advertising without requiring device connections. This template:
- Uses BLE advertising to broadcast device information
- Encodes sensor data as CBOR payloads in BLE manufacturer data packets
- Broadcasts at configurable frequency (default 50Hz)
- Works seamlessly with the [BLE-CBOR-to-OSC script](https://gitlab.com/sat-mtl/collaborations/2024-iot/ble-cbor-to-osc)

**Key Features**:
- **Broadcast without connection**: Receive data from hundreds of BLE devices simultaneously without establishing individual connections
- **Range**: Tested with ~120 devices in a 0-150 metre range with updates every 500ms
- **CBOR encoding**: Efficient binary encoding for minimal payload
- **Device identification**: Each device can be assigned a unique ID via `config.json` for easy tracking

**Use Case**: Ideal for IoT scenarios where you need to monitor many sensor devices simultaneously (e.g., distributed sensor networks, interactive installations) and is well-suited for lower data-rate transmission.

#### Getting Started with BLE Advertising

1. **Build and Upload**: Build and upload the firmware to your ESP32 board
2. **Configure Device**: Optionally modify the device name and ID in `config.json`
3. **Upload Filesystem**: Upload the data folder using the LittleFS upload tool
4. **Run BLE-CBOR-to-OSC Script**: 
   - Clone the [BLE-CBOR-to-OSC repository](https://gitlab.com/sat-mtl/collaborations/2024-iot/ble-cbor-to-osc)
   - Create a Python virtual environment : `python -m venv venv`
   - Activate the venv : `source ./venv/bin/activate`
   - Install Python dependencies: `pip install -r requirements.txt`
   - Run: `python ble-cbor-to-osc.py`
5. **Receive OSC Messages**: The script forwards BLE advertising data as OSC messages to `127.0.0.1:9001` (configurable)

#### BLE Configuration

You can customize the BLE advertising behavior:
- **Frequency**: Modify `target_frequency` variable (default 50Hz)
- **Sensor Data**: Replace dummy `sensor1` and `sensor2` with actual pin readings

For more information, see the [BLE-CBOR-to-OSC script documentation](https://gitlab.com/sat-mtl/collaborations/2024-iot/ble-cbor-to-osc).




