# Web Server

The `puara-module` library includes an integrated web server that allows you to configure your device from a web browser. For more details on the browser pages from a user perspective, see [Browser Pages](browser.md).

---

## Starting the Web Server

The web server is started automatically when you call the `puara.start()` method. You can also start and stop it manually:

```cpp
httpd_handle_t server = puara.start_webserver();
puara.stop_webserver();
```

---

## Accessing the Web Interface

Once the web server is running, you can access it in two ways:

1. **Via IP Address**: Navigate to the device's IP address in your web browser (e.g., `http://192.168.4.1` for AP mode, or the assigned IP in STA mode which can be retrieved using `puara.staIP()`).

2. **Via mDNS Hostname**: If mDNS is enabled, you can access the device using its hostname (e.g., `http://your-device-name.local`).

---

## Web Pages

The web pages are stored in the `data` directory of your project. The `puara-module` library provides a set of default web pages that you can customize to suit your needs.

### Default Pages

| File | Description |
|------|-------------|
| `index.html` | The main page of the web interface where users can configure WiFi SSID and device settings. |
| `scan.html` | Displays available WiFi networks for connection. |
| `settings.html` | Allows users to configure custom variable values for the device. |
| `update.html` | Reserved for future Over-The-Air (OTA) update functionality. |
| `reboot.html` | Confirmation page displayed when the device is rebooting. |
| `saved.html` | Confirmation page displayed after settings are saved. This triggers the callback registered via `puara.set_settings_changed_handler()`. |

### Styling

The `style.css` file in the `data` directory controls the appearance of all web pages. You can modify this file to customize the look and feel of the web interface.

---

## Customizing Web Pages

You can edit the HTML files in the `data` directory to change the appearance and functionality of the web interface. The web pages use a simple template system that allows you to insert dynamic values from your device into the HTML.

> **Note:** After modifying files in the `data` directory, you need to re-upload the filesystem to your device. See [Filesystem](filesystem.md) for instructions on uploading filesystem data. 