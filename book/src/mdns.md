# mDNS

The `puara-module` library provides a simple interface for using the mDNS service.

## Starting the mDNS service

The mDNS service is started automatically when you call the `puara.start()` method. You can also start it manually using the `start_mdns_service()` method:

```cpp
puara.start_mdns_service("my-device", "My Device");
```

The `start_mdns_service()` method takes two arguments:

*   `device_name`: The name of the device.
*   `instance_name`: The name of the mDNS instance.

When the mDNS service is running, you can access your device by its name instead of its IP address. For example, you can navigate to `http://my-device.local` in a web browser.
