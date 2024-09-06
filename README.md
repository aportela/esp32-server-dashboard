# esp32-server-dashboard

## Introduction

## Supported hardware

### Display

Any **SPI** display that is supported by the driver **ST7789V** with a resolution of **240x320**

I'm using displays like these: [front](https://github.com/user-attachments/assets/83cdd284-0a36-4b31-b1f5-8871a9defa79) | [back](https://github.com/user-attachments/assets/6a91112b-fccb-408f-8608-26e1bab05a36) (the cost is about 3.5 euros):

### Microcontroller

In theory you should be able to use any esp32 as long as it has enough pins

I'm using these two:

ESP32-WROOM-32D: [front]() | [back]()

ESP32-C3 SuperMini: [front]() | [back]()

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
