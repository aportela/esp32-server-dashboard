# esp32-server-dashboard

## Introduction

A small project to display in real time stats (cpu/memory/temperature/network bandwidth usage) of the system (using telegraf to collect data & mqtt to transport messages) on my servers while I was getting started in microcontroller programming.

## Supported hardware

### Display

Any **SPI** display that is supported by the driver **ST7789V** with a resolution of **240x320**.

I'm using displays like these (the cost is about 3.5 euros):

<img src="https://github.com/user-attachments/assets/83cdd284-0a36-4b31-b1f5-8871a9defa79" width="118" alt="front" />
<img src="https://github.com/user-attachments/assets/6a91112b-fccb-408f-8608-26e1bab05a36" width="100" alt="back" />

### Microcontroller

In theory you should be able to use any esp32 as long as it has enough pins.

I'm using these two:

ESP32-WROOM-32D (the cost is about 4 euros):

<img src="https://github.com/user-attachments/assets/32d537b2-a7ee-48ea-b386-2d484f4e1792" width="150" alt="front" />
<img src="https://github.com/user-attachments/assets/44905c15-85be-4a14-82b4-89ff0571282d" width="185" alt="pinout" />

ESP32-C3 SuperMini (the cost is about 2 euros):

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

Switch/button contacts are connected to ESP32-C3 SuperMini PIN 16 and GND.

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

Switch/button contacts are connected to ESP32-C3 SuperMini PIN 10 and GND.

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

Configuration file (**telegraf.conf**) file using data collection interval = 1s

```
[global_tags]
[agent]
  interval = "1s"
  round_interval = true
  metric_batch_size = 1000
  metric_buffer_limit = 10000
  collection_jitter = "0s"
  flush_interval = "1s"
  flush_jitter = "0s"
  precision = "0s"
  hostname = ""
  omit_hostname = false

[[outputs.mqtt]]
servers = ["192.168.1.1:1883", ]
topic = "telegraf/{{ .Hostname }}/{{ .PluginName }}"
username = "mqtt_username"
password = "mqtt_password"

[[inputs.cpu]]
  percpu = false
  totalcpu = true
  collect_cpu_time = false
  report_active = false
  core_tags = false

[[inputs.mem]]

[[inputs.system]]

[[inputs.net]]

interfaces = ["enp1s0"]

# lm-sensors package required for linux systems
[[inputs.sensors]]

# uncomment this for using with windows (WARNING: telegraf must be run as administrator)
#[[inputs.temp]]

# uncomment this for using with OPNSense/FreeBSD systems
#[[inputs.exec]]
# commands = [
#   "sh /usr/local/etc/telegraf-scripts/cputemp-sensors-by-package.sh",
# ]
# timeout = "5s"
# data_format = "influx"

```

Also, this is my script (/usr/local/etc/telegraf-scripts/cputemp-sensors-by-package.sh) for getting/parsing cpu temperatures on OPNSense.

```
#!/bin/sh
sysctl hw.acpi.thermal.tz0.temperature | sed 's/hw\.acpi\.thermal\.tz0\.temperature: /sensors,feature=package_id_0 temp_input=/;s/.$//'
```

### Microcontroller

Configuration is done via serial sending string commands. Any serial terminal software like putty or Arduino IDE serial monitor console can be used.

NOTE: "clear commands" **require whitespace** (separator without value) at end, ex: clear wifi password => "SET_WIFI_PASSWORD ".

| Command                         | Param               | Description                              | Example                                                  |
| ------------------------------- | ------------------- | ---------------------------------------- | -------------------------------------------------------- |
| REBOOT                          |                     | Reboot device                            | **REBOOT**                                               |
| CLEAR_SETTINGS                  |                     | Remove all device settings               | **CLEAR_SETTINGS**                                       |
| EXPORT_SETTINGS                 |                     | Export all device settings               | **EXPORT_SETTINGS**                                      |
| CONNECT_WIFI                    |                     | Connect (manually) to WIFI               | **CONNECT_WIFI**                                         |
| DISCONNECT_WIFI                 |                     | Disconnect (manually) to WIFI            | **DISCONNECT_WIFI**                                      |
| SET_WIFI_SSID                   | <ssid>              | Save WIFI SSID                           | **SET_WIFI_SSID myssid**                                 |
|                                 |                     | Clear WIFI SSID                          | **SET_WIFI_SSID**                                        |
| SET_WIFI_PASSWORD               | <password>          | Save WIFI password                       | **SET_WIFI_PASSWORD secret**                             |
|                                 |                     | Clear WIFI password                      | **SET_WIFI_PASSWORD**                                    |
| SET_MQTT_TELEGRAF_URI           | <uri>               | Save MQTT uri                            | **SET_MQTT_TELEGRAF_URI mqtt://192.168.1.1**             |
|                                 |                     | Clear MQTT uri                           | **SET_MQTT_TELEGRAF_URI**                                |
| SET_MQTT_USERNAME               | <username>          | Save MQTT username                       | **SET_MQTT_USERNAME foobar**                             |
|                                 |                     | Clear MQTT username                      | **SET_MQTT_USERNAME**                                    |
| SET_MQTT_PASSWORD               | <password>          | Save MQTT password                       | **SET_MQTT_PASSWORD secret**                             |
|                                 |                     | Clear MQTT password                      | **SET_MQTT_PASSWORD**                                    |
| SET_MQTT_TELEGRAF_GLOBAL_TOPIC  | <topic>             | Save MQTT topic                          | **SET_MQTT_TELEGRAF_GLOBAL_TOPIC telegraf/myhostname/#** |
|                                 |                     | Clear MQTT topic                         | **SET_MQTT_TELEGRAF_GLOBAL_TOPIC**                       |
| TOGGLE_SCREEN                   |                     | Change screen (info/dashboard)           | **TOGGLE_SCREEN**                                        |
| SET_MAX_DOWNLOAD_BITS_BANDWITH  | <bits>              | Save net download graph fixed max scale  | **SET_MAX_DOWNLOAD_BITS_BANDWITH 1000000000**            |
|                                 |                     | Clear net download graph fixed max scale | **SET_MAX_DOWNLOAD_BITS_BANDWITH**                       |
| SET_MAX_UPLOAD_BITS_BANDWITH    | <bits>              | Save net upload graph fixed max scale    | **SET_MAX_UPLOAD_BITS_BANDWITH 1000000000**              |
|                                 |                     | Clear net upload graph fixed max scale   | **SET_MAX_UPLOAD_BITS_BANDWITH**                         |
| SET_NETWORK_INTERFACE_ID        | <iface>             | Save network interface id                | **SET_NETWORK_INTERFACE_ID enp1s0**                      |
|                                 |                     | Clear network interface id               | **SET_NETWORK_INTERFACE_ID**                             |
| SET_HOSTNAME                    | <hostname>          | Save hostname                            | **SET_HOSTNAME myhostname**                              |
|                                 |                     | Clear hostname                           | **SET_HOSTNAME**                                         |
| SET_SCREEN_MIRROR_FLIP_VERTICAL | <bool>              | Save screen mirror orientation           | **SET_SCREEN_MIRROR_FLIP_VERTICAL true**                 |
|                                 | <bool>              | Clear screen mirror orientation          | **SET_SCREEN_MIRROR_FLIP_VERTICAL false**                |
| SET_DEFAULT_SCREEN              | <screen>            | Save "INFO" as default screen            | **SET_DEFAULT_SCREEN 1**                                 |
|                                 | <screen>            | Save "DASHBOARD" as default screen       | **SET_DEFAULT_SCREEN 2**                                 |
| SET_DASHBOARD_BLOCKS            | <index> <items>     | Set dashboard blocks                     | **SET_DASHBOARD_BLOCKS 3 2,3,6,7,8**                     |
| SET_DASHBOARD_BLOCKS            | <index> <0,0,0,0,0> | Clear dashboard blocks                   | **SET_DASHBOARD_BLOCKS 3 0,0,0,0,0**                     |

#### NOTES:

- Clear MQTT username/password configuration values if your mqtt server/broker do not require authentication.
- SET_MAX_DOWNLOAD_BITS_BANDWITH && SET_MAX_UPLOAD_BITS_BANDWITH configuration values are required to use "NET DOWN" (27) / "NET UP" (28) static, not auto-scale networking blocks (see next section).

#### CUSTOMIZE DASHBOARD CLOCKS

Every dashboard / blocks are customized via serial commands. By default, there is only one dashboard (CPU LOAD, MEM USED, CPU TEMP, DYNAMIC NET DOWN, DYNAMIC NET UPLD) that cannot be deleted but can be modified (block exchange). Dashboard configuration must be sequential (a third one cannot be configured if the second one is not established). To delete a dashboard we only have to specify its index and values ​​0,0,0,0,0

If you want to add more dashboards and the current limit is not enough (currently 8, defined in src/display/DashboardItemType.hpp) you just have to modify the MAX_DASHBOARDS tag and recompile the project

The list of supported block types corresponds to these values ​​(defined in the src/display/DashboardItemType.hpp file):

| TYPE               | VALUE | NOTES                                                                            |
| ------------------ | ----- | -------------------------------------------------------------------------------- |
| NONE               | 0     | empty value, used for removing block                                             |
| CPU LOAD           | 1     |                                                                                  |
| CPU USER           | 2     |                                                                                  |
| CPU SYSTEM         | 3     |                                                                                  |
| CPU IDLE           | 4     |                                                                                  |
| CPU ACTIVE         | 5     |                                                                                  |
| CPU NICE           | 6     |                                                                                  |
| CPU IOWAIT         | 7     |                                                                                  |
| CPU IRQ            | 8     |                                                                                  |
| CPU SOFT IRQ       | 9     |                                                                                  |
| CPU STEAL          | 10    |                                                                                  |
| CPU GUEST          | 11    |                                                                                  |
| CPU GUEST NICE     | 12    |                                                                                  |
| MEM ACTIVE         | 13    |                                                                                  |
| MEM AVAILABLE      | 14    |                                                                                  |
| MEM BUFFERED       | 15    |                                                                                  |
| MEM CACHED         | 16    |                                                                                  |
| MEM DIRTY          | 17    |                                                                                  |
| MEM FREE           | 18    |                                                                                  |
| MEM INACTIVE       | 19    |                                                                                  |
| MEM LAUNDRY        | 20    |                                                                                  |
| MEM MAPPED         | 21    |                                                                                  |
| MEM SHARED         | 22    |                                                                                  |
| MEM SWAP CACHED    | 23    |                                                                                  |
| MEM SWAP FREE      | 24    |                                                                                  |
| MEM USED           | 25    |                                                                                  |
| CPU TEMP           | 26    |                                                                                  |
| NET DOWN           | 27    | requires having a **bit limit** set with the **SET_MAX_DOWNLOAD_BITS_BANDWITH**  |
|                    |       | y-axis units are defined in multiples of **bits/sec** (using previous max limit) |
|                    |       | current value units are defined in multiples of **bytes/sec**                    |
| NET UP             | 28    | requires having a **bit limit** set with the **SET_MAX_UPLOAD_BITS_BANDWITH**    |
|                    |       | y-axis units are defined in multiples of **bits/sec** (using previous max limit) |
|                    |       | current value units are defined in multiples of **bytes/sec**                    |
| NET DOWN (DYNAMIC) | 29    | y-axis automatically auto-scales with values ​​over time                         |
|                    |       | y-axis units are defined in multiples of **bytes/sec**                           |
|                    |       | current value units are defined in multiples of **bytes/sec**                    |
| NET UP (DYNAMIC)   | 30    | y-axis automatically auto-scales with values ​​over time                         |
|                    |       | y-axis units are defined in multiples of **bytes/sec**                           |
|                    |       | current value units are defined in multiples of **bytes/sec**                    |

#### NOTES:

- Not all blocks are supported on all operating systems.
- All network blocks share the same interface (support for multiple interfaces is not available at this time). If we do not specify a single network interface in the telegraf configuration, data will arrive from several interfaces and if one is not configured via serial commands (SET_NETWORK_INTERFACE_ID), they will all be mixed and the data displayed will be incorrect.

## Working screenshots / photos

<img src="https://github.com/user-attachments/assets/80b8c9d7-9a19-48ca-8666-64d6b4916eb6" width="400" alt="front" />
<img src="https://github.com/user-attachments/assets/96fe35ef-6684-4bc2-a7de-d487f830032d" width="400" alt="back" />
<img src="https://github.com/user-attachments/assets/a9fe69d1-ba4f-44a1-9da7-592133bb7946" width="400" alt="mounted info screens" />
<img src="https://github.com/user-attachments/assets/5b09ed09-5b3a-4f2f-8699-2b766188d886" width="400" alt="mounted dashboard screens" />
<img src="https://github.com/user-attachments/assets/5d25049c-14d9-4898-9be4-448c936971bd" width="400" alt="dashboard view" />
<img src="https://github.com/user-attachments/assets/aab87baa-0ce6-44a4-9beb-0d1dafc8224b" width="400" alt="info view" />
