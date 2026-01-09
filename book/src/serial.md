# Serial

The `puara-module` library provides a simple interface for serial communication.

## Starting serial listening

The serial listening is started automatically when you call the `puara.start()` method. You can also start it manually using the `start_serial_listening()` method:

```cpp
puara.start_serial_listening();
```

When serial listening is active, the `puara-module` will listen for incoming data on the serial port.

## Sending serial data

You can send data over the serial port using the `send_serial_data()` method:

```cpp
puara.send_serial_data("Hello, world!");
```

## Monitor

You can specify the monitor to be used for logging when you call the `start()` method. The available monitors are:

*   `PuaraAPI::UART_MONITOR`: Logs to the serial port.
*   `PuaraAPI::TELNET_MONITOR`: Logs to a Telnet server.
*   `PuaraAPI::NO_MONITOR`: Disables logging.

The default monitor is `PuaraAPI::UART_MONITOR`.
