# Installing Puara-Modules and related dependencies

This chapter will guide you through the process of installing the `puara-module` library for two different development environments : [Arduino IDE](https://docs.arduino.cc/software/ide/) and [PlatformIO via VSCode](https://platformio.org/platformio-ide). There will also be information about the related dependencies and how to install them.

To run a project, you only need one of two approaches : Arduino or PlatformIO. For new users, we recommend using the Arduino approach as all is packaged to facilitate building and uploading in less steps than PlatformIO. For a more granular approach to development, PlatformIO may be more suited for your needs. 


## 1. Arduino approach 

### 1.1 Arduino 2.0 IDE and ESP board libraries

If not already done, download and install [Arduino 2.0](https://www.arduino.cc/en/software/) and the ESP32 board librairies. For the former, follow the install steps from the referred Arduino website. For the latter, follow these steps : 
1. Open Arduino 2.0 IDE;
2. Click on the **Boards Manager** icon in left tab (second from the top) OR navigate to  `Tools > Board:"" > Boards Manager... `;
3. In the Boards Manager **search bar**, type **esp32** and install both *'Arduino ESP32 Boards' by Arduino* and *'esp32' by Espressif Systems*.


### 1.2 Setup filesystem uploader

As mentionned in the summary at [Important detail for users](SUMMARY.md#important-detail-for-users), you must also upload the filesystem alongside the built and uploaded code. In order to do so in Arduino IDE, we need the [arduino-littlefs-upload](https://github.com/earlephilhower/arduino-littlefs-upload) software tool as Arduino does not yet offer this option natively. 

You may follow the steps defined [here](https://github.com/earlephilhower/arduino-littlefs-upload) which are mainly :
- Download the [VSIX file](https://github.com/earlephilhower/arduino-littlefs-upload/releases)
- Copy/Move the file to appropriate destination :
    - For Mac or Linux users, move file to `~/.arduinoIDE/plugins/`
    - For Windows users, move file to `C:\Users\<username>\.arduinoIDE\plugins\`
        (you may need to make this directory yourself beforehand). 
-Restart the IDE. 

Once these steps are done, you will be able to upload the filesystem by following these steps on [Usage, uploading a filesystem to the device](https://github.com/earlephilhower/arduino-littlefs-upload/releases) which consists mainly of pressing the following keys and selecting the appropriate option : 

- For Linux or Windows users :
    - press `[Ctrl] + [Shift] + [P]` to open the Command Palette in the Arduino IDE, then select *"Upload LittleFS to Pico/ESP8266/ESP32"*.
- For macOS users :
    - press `[⌘] + [Shift] + [P]` to open the Command Palette in the Arduino IDE, then *"Upload LittleFS to Pico/ESP8266/ESP32"*.



### 1.3 Puara Module Library and Examples 
For an Arduino install of the Puara Module ressources, follow the next steps : 

1.  Download the `puara-module` library as a ZIP file from the [GitHub repository](https://github.com/Puara/puara-module) by clicking the `<> Code` button and selecting *Download ZIP* option.
2.  In the Arduino IDE, go to `Sketch > Include Library > Add .ZIP Library...` and select the downloaded ZIP file.
3.  The examples will be available in `File > Examples > puara-module`.


This concludes the basic installation of Puara Modules and related dependencies for Arduino users. If you are using this approach, you may skip the following text in this document and go directly to [Configuring your board and network specifications](configuration.md) page. 




## 2. PlatformIO Approach

1.  Clone the `puara-module` repository:
    ```bash
    git clone https://github.com/sat-mtl/puara-module.git
    ```
2.  Open the `puara-module` folder in Visual Studio Code with the PlatformIO extension installed.
3.  The examples are located in the `examples/` directory. You can open one of the examples and upload it to your ESP32 board.
