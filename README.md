# KnxBridge -  KNX bridge for HomeKit and Philips-Hue Emulation

This bridge emulates a HomeKit-Bridge and a Philips-Hue bridge.
The SW is in alpha version state! Use on your own risk.

## HW

KNX Bridge based on 
- ESP32 DevKit Board (I use the [AZ-Delivery ESP32 Dev Kit C](https://www.az-delivery.de/en/products/esp32-developmentboard), but other boards should also be fine, maybe the LED and Program button key have to be remapped)
- [NanoBCU](https://github.com/OpenKNX/OpenKNX/wiki/NanoBCU) 
- USB power adapter

Connect ESP32 GND with GND of the NanoBCU
Connect ESP32 GPIO 16 with the Tx Pin of the NanoBCU
Connect ESP32 GPIO 17 with the Rx Pin of the NanoBCU

Because the ESP32 need more power than the NanoBCU provides, an extra power supply with an USB power adapter connected to the ESP32 board is required.

## Sources

To load the sources to your development environment clone the repo including submodule:

```
git clone --recurse-submodules https://github.com/mgeramb/KnxBridge.git
```

## ETS application

The ETS application has to be created out of the KnxBridge.xml file with the [CreateKnxProd](https://github.com/thelsing/CreateKnxProd) tool

## Firmware

The firmeware can be compiled and uploaded in VS Code with the [platformioIO](https://docs.platformio.org/en/latest/integration/ide/vscode.html#ide-vscode) extension installed.

## Known Issues

- Do not use special characters like 'äöüß...' for the device names.
- Philips Hue emulations uses a dimmer instead of a switch because of the used base library [Aircoookie/Espalexa](https://github.com/Aircoookie/Espalexa)
