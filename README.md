# esp32-server-dashboard

## Introduction

## Supported hardware

### Display

Any **SPI** display that is supported by the driver **ST7789V** with a resolution of **240x320**

I'm using displays like these (the cost is about 3.5 euros):

<img src="https://github.com/user-attachments/assets/83cdd284-0a36-4b31-b1f5-8871a9defa79" width="100" alt="front" />
<img src="https://github.com/user-attachments/assets/6a91112b-fccb-408f-8608-26e1bab05a36" width="100" alt="back" />

### Microcontroller

In theory you should be able to use any esp32 as long as it has enough pins

I'm using these two:

ESP32-WROOM-32D:

<img src="https://github.com/user-attachments/assets/32d537b2-a7ee-48ea-b386-2d484f4e1792" width="150" alt="front" />
<img src="https://github.com/user-attachments/assets/44905c15-85be-4a14-82b4-89ff0571282d" width="150" alt="pinout" />

ESP32-C3 SuperMini:

<img src="https://github.com/user-attachments/assets/55c3a8f9-fd53-4444-91f7-219cf6f70d87" width="224" alt="front" />
<img src="https://github.com/user-attachments/assets/d10aa28f-d15f-4f76-9b0d-2c3fa63a687d" width="100" alt="back" />
<img src="https://github.com/user-attachments/assets/1b10c2a7-7288-4ec2-89c7-77f9d9c0241b" width="260" alt="pinout" />

### Switch / Button

Any button / switch will work

## Install (Hardware)

### ESP32 WROOM

### ESP32-C3 Supermini

## Install (Software)

Services:

- [Mosquitto](https://mosquitto.org/) (should work with any other server/broker)

Arduino IDE Board setup:

- ESP32 WROOM: **ESP32-WROOM-DA Module**

- ESP32-C3 SuperMini **ESP32C3 Dev Module**
  - Tools => **USB CDC On Boot** => "**Enabled**" (required for serial communication)

Arduino IDE required libraries:

- [LovyanGFX by lovyan03](https://github.com/lovyan03/LovyanGFX) (ST7789V display driver)

## Configuration

### MQTT

[Authentication with usernames and passwords](https://mosquitto.org/documentation/authentication-methods/) (esp32-server-dashboard it also works without MQTT authentication)

### Telegraf

### Microcontroller

Configuration is done via serial

## Working screenshots / photos
