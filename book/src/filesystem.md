# Filesystem

The `puara-module` library provides a simple interface for working with the filesystem. It supports both LittleFS and SPIFFS.

## Mounting the filesystem

The filesystem is mounted automatically when you call the `puara.start()` method. You can also mount and unmount it manually using the `mount()` and `unmount()` methods:

```cpp
puara.mount();
puara.unmount();
```

## Reading and writing files

You can read and write files using the `read_config_json()`, `write_config_json()`, `read_settings_json()`, and `write_settings_json()` methods. These methods are used to manage the `config.json` and `settings.json` files.

If you want to work with other files, you can use the standard `LittleFS` or `SPIFFS` APIs.

## LittleFS vs SPIFFS

By default, the `puara-module` library uses LittleFS. If you want to use SPIFFS, you need to uncomment the following line in `puara_config.hpp`:

```cpp
// #define PUARA_FILESYSTEM_SPIFFS
```

And comment out the following line:

```cpp
#define PUARA_FILESYSTEM_LITTLEFS
```
