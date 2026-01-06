# Installing Puara-Modules

This chapter will guide you through the process of installing the `puara-module` library for two different development environments : [Arduino IDE](https://docs.arduino.cc/software/ide/) and [PlatformIO via VSCode](https://platformio.org/platformio-ide). There will also be information about it's related dependencies where needed.

An important fact to know before jumping into this project is that there is a dual uploading process done in two distinct steps contrary to most Arduino or embedded development projects that only need to build and upload. 
Our proposed library includes executable code AND a filesystem. 


## 1. Arduino Approach 

### Arduino 2.0 IDE and ESP Board Libraries

If not already done, download and install [Arduino 2.0](https://www.arduino.cc/en/software/) and get ESP32 board support. For the former, follow the isntall steps from the referred website, as for the latter, follow these steps : 
    1 - Open Arduino 2.0 IDE;
    2 - Click on the "Boards Manager" icon in left tab (second from the top) OR navigate to : Tools/Board:""/Boards Manager
    3 - In the Boards Manager search bar, type "esp32" and install both 'Arduino ESP32 Boards' by Arduino and 'esp32' by Espressif Systems.



### Puara Library and Examples 
1.  Download the `puara-module` library as a ZIP file from the [GitHub repository](https://github.com/sat-mtl/puara-module).
2.  In the Arduino IDE, go to `Sketch > Include Library > Add .ZIP Library...` and select the downloaded ZIP file.
3.  The examples will be available in `File > Examples > puara-module`.

### Uploading the Filesystem

## 2. PlatformIO Approach

1.  Clone the `puara-module` repository:
    ```bash
    git clone https://github.com/sat-mtl/puara-module.git
    ```
2.  Open the `puara-module` folder in Visual Studio Code with the PlatformIO extension installed.
3.  The examples are located in the `examples/` directory. You can open one of the examples and upload it to your ESP32 board.
