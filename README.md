# esp32-server-dashboard

## Introduction

## Supported hardware

### Display

Any **SPI** display that is supported by the driver **ST7789V** with a resolution of **240x320**

I'm using displays like these (the cost is about 3.5 euros):

<img src="https://github.com/user-attachments/assets/83cdd284-0a36-4b31-b1f5-8871a9defa79" width="118" alt="front" />
<img src="https://github.com/user-attachments/assets/6a91112b-fccb-408f-8608-26e1bab05a36" width="100" alt="back" />

### Microcontroller

In theory you should be able to use any esp32 as long as it has enough pins

I'm using these two:

ESP32-WROOM-32D:

<img src="https://github.com/user-attachments/assets/32d537b2-a7ee-48ea-b386-2d484f4e1792" width="150" alt="front" />
<img src="https://github.com/user-attachments/assets/44905c15-85be-4a14-82b4-89ff0571282d" width="185" alt="pinout" />

ESP32-C3 SuperMini:

<img src="https://github.com/user-attachments/assets/55c3a8f9-fd53-4444-91f7-219cf6f70d87" width="224" alt="front" />
<img src="https://github.com/user-attachments/assets/d10aa28f-d15f-4f76-9b0d-2c3fa63a687d" width="100" alt="back" />
<img src="https://github.com/user-attachments/assets/1b10c2a7-7288-4ec2-89c7-77f9d9c0241b" width="250" alt="pinout" />

### Switch / Button

Any button / switch will work

## Install (Hardware)

### ESP32-WROOM-32D

| ESP32-WROOM-32D | ST7789V |
| --------------- | ------- |
| PIN 5           | CS      |
| PIN 4           | RST     |
| PIN 2           | DC      |
| PIN 23          | SDA     |
| PIN 18          | SCL     |
| GND             | GND     |
| 3.3V            | VCC     |

Switch/button contacts are connected to ESP32-C3 SuperMini PIN 16 and GND

### ESP32-C3 SuperMini

| ESP32-C3 SuperMini | ST7789V |
| ------------------ | ------- |
| PIN 5              | CS      |
| PIN 9              | RST     |
| PIN 8              | DC      |
| PIN 6              | SDA     |
| PIN 4              | SCL     |
| GND                | GND     |
| 3.3V               | VCC     |

Switch/button contacts are connected to ESP32-C3 SuperMini PIN 10 and GND

## Install (Software)

Services:

- [Mosquitto](https://mosquitto.org/) (should work with any other server/broker)

Arduino IDE Board setup:

- ESP32-WROOM-32D: **ESP32-WROOM-DA Module**

- ESP32-C3 SuperMini **ESP32C3 Dev Module**
  - Tools => **USB CDC On Boot** => "**Enabled**" (required for serial communication)

Arduino IDE required libraries:

- [LovyanGFX by lovyan03](https://github.com/lovyan03/LovyanGFX) (ST7789V display driver)
- [Bounce2 by Thomas Ouellet Fredericks](https://github.com/thomasfredericks/Bounce2) (Debouncing library for Arduino and Wiring)

## Configuration

### MQTT

[Authentication with usernames and passwords](https://mosquitto.org/documentation/authentication-methods/) (esp32-server-dashboard it also works without MQTT authentication)

### Telegraf

### Microcontroller

Configuration is done via serial sending string commands. Any serial terminal software like putty or Arduino IDE serial monitor console can be used

| Command                         | Param      | Description                              | Example                                                  |
| ------------------------------- | ---------- | ---------------------------------------- | -------------------------------------------------------- |
| REBOOT                          |            | Reboot device                            | **REBOOT**                                               |
| CLEAR_SETTINGS                  |            | Remove all device settings               | **CLEAR_SETTINGS**                                       |
| EXPORT_SETTINGS                 |            | Export all device settings               | **EXPORT_SETTINGS**                                      |
| CONNECT_WIFI                    |            | Connect (manually) to WIFI               | **CONNECT_WIFI**                                         |
| DISCONNECT_WIFI                 |            | Disconnect (manually) to WIFI            | **DISCONNECT_WIFI**                                      |
| SET_WIFI_SSID                   | <ssid>     | Save WIFI SSID                           | **SET_WIFI_SSID myssid**                                 |
|                                 |            | Clear WIFI SSID                          | **SET_WIFI_SSID**                                        |
| SET_WIFI_PASSWORD               | <password> | Save WIFI password                       | **SET_WIFI_PASSWORD secret**                             |
|                                 |            | Clear WIFI password                      | **SET_WIFI_PASSWORD**                                    |
| SET_MQTT_TELEGRAF_URI           | <uri>      | Save MQTT uri                            | **SET_MQTT_TELEGRAF_URI mqtt://192.168.1.1**             |
|                                 |            | Clear MQTT uri                           | **SET_MQTT_TELEGRAF_URI**                                |
| SET_MQTT_USERNAME               | <username> | Save MQTT username                       | **SET_MQTT_USERNAME foobar**                             |
|                                 |            | Clear MQTT username                      | **SET_MQTT_USERNAME**                                    |
| SET_MQTT_PASSWORD               | <password> | Save MQTT password                       | **SET_MQTT_PASSWORD secret**                             |
|                                 |            | Clear MQTT password                      | **SET_MQTT_PASSWORD**                                    |
| SET_MQTT_TELEGRAF_GLOBAL_TOPIC  | <topic>    | Save MQTT topic                          | **SET_MQTT_TELEGRAF_GLOBAL_TOPIC telegraf/myhostname/#** |
|                                 |            | Clear MQTT topic                         | **SET_MQTT_TELEGRAF_GLOBAL_TOPIC**                       |
| TOGGLE_SCREEN                   |            | Change screen (info/dashboard)           | **TOGGLE_SCREEN**                                        |
| SET_MAX_DOWNLOAD_BYTES_BANDWITH | <bytes>    | Save net download graph fixed max scale  | **SET_MAX_DOWNLOAD_BYTES_BANDWITH 10737418240**          |
|                                 |            | Clear net download graph fixed max scale | **SET_MAX_DOWNLOAD_BYTES_BANDWITH**                      |
| SET_MAX_UPLOAD_BYTES_BANDWITH   | <bytes>    | Save net upload graph fixed max scale    | **SET_MAX_UPLOAD_BYTES_BANDWITH 10737418240**            |
|                                 |            | Clear net upload graph fixed max scale   | **SET_MAX_UPLOAD_BYTES_BANDWITH**                        |
| SET_NETWORK_INTERFACE_ID        | <iface>    | Save network interface id                | **SET_NETWORK_INTERFACE_ID enp1s0**                      |
|                                 |            | Clear network interface id               | **SET_NETWORK_INTERFACE_ID**                             |
| SET_HOSTNAME                    | <hostname> | Save hostname                            | **SET_HOSTNAME myhostname**                              |
|                                 |            | Clear hostname                           | **SET_HOSTNAME**                                         |
| SET_SCREEN_MIRROR_FLIP_VERTICAL | <bool>     | Save screen mirror orientation           | **SET_SCREEN_MIRROR_FLIP_VERTICAL true**                 |
|                                 | <bool>     | Clear screen mirror orientation          | **SET_SCREEN_MIRROR_FLIP_VERTICAL false**                |
| SET_DEFAULT_SCREEN              | <screen>   | Save "INFO" as default screen            | **SET_DEFAULT_SCREEN 1**                                 |
|                                 | <screen>   | Save "DASHBOARD" as default screen       | **SET_DEFAULT_SCREEN 2**                                 |

#### NOTES:

- Clear MQTT username/password configuration values if your mqtt server/broker do not require authentication
- Clear max download/upload bytes bandwidth configuration values to using "dynamic scales" (auto grow/resize)

## Working screenshots / photos

<img src="https://github.com/user-attachments/assets/baf19f9b-25b6-4e35-a8fd-df9bde0e4b76" width="520" alt="front" />
<img src="https://github.com/user-attachments/assets/7c8c8f43-0513-4fd8-a013-28c0942908a7" width="520" alt="back" />
<img src="https://github.com/user-attachments/assets/c15abe16-92fa-49a4-8b5f-bc6f7a91d519" width="520" alt="mounted info screens" />
<img src="https://github.com/user-attachments/assets/32fa1f35-790d-4064-8349-2522e529c434" width="520" alt="mounted dashboard screens" />
<img src="https://github.com/user-attachments/assets/872e95f5-7a2e-43ea-80d9-61fec984987c" width="520" alt="dashboard view" />
<img src="https://github.com/user-attachments/assets/6a5c7708-3229-4724-87d5-83b9e114a6df" width="520" alt="info view" />
