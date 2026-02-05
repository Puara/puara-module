# Serial and Debug Logging

The `puara-module` library provides a simple interface for serial communication, allowing you to send data and receive commands via the serial port.

---

## Starting Serial Listening

Serial listening is started automatically when you call `puara.start()`. You can also start it manually:

```cpp
bool success = puara.start_serial_listening();
```

**Returns:** `true` if serial listening was successfully started, `false` otherwise.

When serial listening is active, the device will listen for incoming commands on the serial port, allowing you to interact with and configure the device via a serial terminal.

---

## Sending Serial Data

You can send data over the serial port using the `send_serial_data()` method:

```cpp
puara.send_serial_data("Hello, world!");
```

---

## Monitor Types

You can specify the serial monitor type when calling the `start()` method:

```cpp
// Use JTAG monitor with debug-level logging
puara.start(PuaraAPI::JTAG_MONITOR, ESP_LOG_DEBUG);
```

### Available Monitors

| Monitor | Description |
|---------|-------------|
| `PuaraAPI::UART_MONITOR` | Standard UART serial monitor (default). |
| `PuaraAPI::JTAG_MONITOR` | JTAG-based debugging monitor. |
| `PuaraAPI::USB_MONITOR` | USB serial monitor. |

### Debug Levels

The second parameter of `start()` sets the ESP-IDF logging level:

| Level | Description |
|-------|-------------|
| `ESP_LOG_NONE` | No logging output. |
| `ESP_LOG_ERROR` | Only error messages. |
| `ESP_LOG_WARN` | Warnings and errors (default). |
| `ESP_LOG_INFO` | Informational messages, warnings, and errors. |
| `ESP_LOG_DEBUG` | Debug messages and all above. |
| `ESP_LOG_VERBOSE` | All messages including verbose output. |
