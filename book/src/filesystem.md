# Filesystem

The `puara-module` library provides a simple interface for working with the filesystem. It supports both **LittleFS** (default) and **SPIFFS**, though the available options depend on your development environment.

---

## Mounting the Filesystem

The filesystem is mounted automatically when you call the `puara.start()` method. You can also mount and unmount it manually using the `mount()` and `unmount()` methods:

```cpp
puara.mount();
puara.unmount();
```

---

## Reading and Writing Files

You can read and write configuration files using the following methods:

- `read_config_json()` / `write_config_json()` - Manage the `config.json` file (device configuration)
- `read_settings_json()` / `write_settings_json()` - Manage the `settings.json` file (user settings)

For working with other files, you can use the standard `LittleFS` or `SPIFFS` APIs directly.

---

## LittleFS vs SPIFFS

By default, the `puara-module` library uses **LittleFS**, which offers better performance and wear leveling compared to SPIFFS.

### Arduino IDE

When using the **Arduino IDE**, only **LittleFS** is supported. You must use the [arduino-littlefs uploader tool](https://github.com/earlephilhower/arduino-littlefs-upload) to upload the filesystem data to your device.

### PlatformIO

When using **PlatformIO**, you have the flexibility to choose between LittleFS (default) and SPIFFS.

#### Default Configuration (LittleFS)

By default, PlatformIO projects are configured to use LittleFS. No changes are required.

#### Switching to SPIFFS

To use SPIFFS instead of LittleFS, you need to modify two settings in your `platformio.ini` file:

**1. Change the filesystem type:**

```ini
; to use spiffs, comment out "board_build.filesystem = littlefs", 
; uncomment "board_build.filesystem = spiffs" and "-DPUARA_SPIFFS"
;board_build.filesystem = littlefs
board_build.filesystem = spiffs
```

**2. Add the SPIFFS build flag:**

```ini
build_flags =
    -DBOARD_HAS_PSRAM
    -DPUARA_SPIFFS    ; compilation flag to indicate the usage of spiffs lib instead of littlefs
```

> **Note:** Make sure both settings are changed together. The `board_build.filesystem` setting determines how PlatformIO uploads the filesystem data, while the `-DPUARA_SPIFFS` build flag tells the puara-module library to use the SPIFFS API at compile time.
