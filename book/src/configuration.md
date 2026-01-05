# Configuration

The `puara-module` library uses two JSON files for configuration: `config.json` and `settings.json`. These files are located in the `data` directory of your project.

## `config.json`

The `config.json` file is used to store the main configuration of your device. Here is an example of a `config.json` file:

```json
{
    "dmi_name": "puara-test",
    "version": 1,
    "port1": 8000,
    "port2": 9000,
    "local_port": 10000,
    "persistent_AP": false
}
```

*   `dmi_name`: The name of your device.
*   `version`: The version of your application.
*   `port1`: The primary port for your application.
*   `port2`: The secondary port for your application.
*   `local_port`: The local port for your application.
*   `persistent_AP`: If `true`, the device will always start in AP mode. If `false`, it will try to connect to a known WiFi network.

## `settings.json`

The `settings.json` file is used to store the settings of your application. You can add any key-value pairs to this file. Here is an example of a `settings.json` file:

```json
{
    "Hitchhiker": "Arthur Dent",
    "answer_to_everything": 42,
    "variable3": 3.14
}
```

You can access the values of these variables from your code using the `getVarNumber()` and `getVarText()` methods of the `Puara` object:

```cpp
float my_float_var = puara.getVarNumber("variable3");
std::string my_string_var = puara.getVarText("Hitchhiker");
```
