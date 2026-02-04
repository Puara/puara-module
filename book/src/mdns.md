# mDNS

The `puara-module` library includes built-in support for **mDNS** (multicast DNS), allowing your device to be discovered and accessed on the local network using a human-readable hostname instead of an IP address.

---

## What is mDNS?

mDNS is a protocol that resolves hostnames to IP addresses within small networks without requiring a dedicated DNS server. It enables you to access your device using a `.local` domain name, making it much easier to connect to your device without needing to know its IP address.

---

## Automatic Configuration

When you call `puara.start()`, the mDNS service is started automatically using the device's **DMI name** (a combination of `device` and `id` from your `config.json`).

For example, with this configuration:

```json
{
    "device": "Puara",
    "id": 1
}
```

The device will be accessible at: **`http://puara_001.local/`**

---

## Manual Configuration

If you need to start the mDNS service manually with a custom name, use the `start_mdns_service()` method:

```cpp
puara.start_mdns_service("my-device", "My Device Instance");
```

### Parameters

| Parameter | Description |
|-----------|-------------|
| `device_name` | The hostname for mDNS discovery. This becomes the `.local` address (e.g., `my-device` → `http://my-device.local/`). |
| `instance_name` | A human-readable name for the service instance, visible in network discovery tools. |

---

## Accessing Your Device

Once mDNS is running, you can access your device's web interface by typing the hostname followed by `.local` in your browser:

```
http://<device_name>.local/
```

For example:
- `http://puara_001.local/`
- `http://my-sensor.local/`

---

## Troubleshooting

### Device not found via `.local` address?

- **Same network**: Ensure your computer and the device are on the same network.
- **mDNS support**: Some networks (especially corporate/enterprise) block mDNS traffic. Try using the IP address directly instead.
- **Windows**: Older Windows versions may require [Bonjour](https://support.apple.com/bonjour) to be installed for `.local` resolution.
- **Firewall**: Check that your firewall allows mDNS traffic (UDP port 5353).

> **Tip:** If you're having trouble with mDNS, you can always access the device via its IP address. Use `puara.staIP()` to retrieve the IP address in your code, or check your router's connected devices list.
