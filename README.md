# OpenKNX Smart Home Bridge

Die OpenKNX Smart Home Bridge erlaubt KNX Geräte über Apple Home und Amazon Alexa zu steuern.

## Features
- SW-Updates können über WLAN (OTA) eigespielt werden
- Hue Emulation für Amazon Alex für: Schaltbare und dimmbare Lampen
- Hue Emulation für Rollanden und Jalousien, dargestellt als dimmbare Lampen
- Homekit Unterstütze Geräte: Schaltbare Lampe, Dimmbare Lampe, Rollläden, Jalousien, Anzeige, Thermostat, Sensoren, Türen, Fenster, Gargagentor

## HW

Empfohlene Hardware:

- [Adafruit ESP32 Feather V2 - 8MB Flash + 2 MB PSRAM](https://www.adafruit.com/product/5400)
- [NanoBCU](https://github.com/OpenKNX/OpenKNX/wiki/NanoBCU) Bezugsquelle: [https://muster.ing-dom.de](https://muster.ing-dom.de/Busankoppler/NanoBCU-Standard.html)
- USB Netzteil (Notwendig, das der Strom der NanoBCU nicht ausreichend ist)

Am Besten verlötet man die NanoBCU wie im Foto mit einer 6 poligen Pfostenleiste.


| ![ESP32 FeatherV2 Aufbau - Ansicht 2](doc/pic/FeatherV2-2.jpg) | ![ESP32 FeatherV2 Aufbau - Ansicht 1](doc/pic/FeatherV2-1.jpg) | ![ESP32 FeatherV2 Aufbau - Ansicht 3](doc/pic/FeatherV2-3.jpg) |
|-|-|-|

Dabei müssen folgende Pins verlötet werden:

| NanoBCU | ESP32           |
|---------|-----------------|
| KNX     | NC              |
| GND     | GND             | 
| SAV     | GPIO26 (A0)     |
| RX      | GPIO25 (A1)     |
| TX      | GPIO34 (A2)     |
| 5V      | nicht verbinden |

![ESP32 Feahter V2 Pinout](doc/pic/adafruit_products_Adafruit_ESP32_Feather_V2_Pinout.jpg)

Die KNX Klemme kann mit Doppelseitigem Klebeband ebenfalls auf dem Board plaziert werden und mit einem Stück Installationsdraht mit den GND und KNX Pin der NanoBCU verlötet werden.

Andere ESP32 Hardware kann benutzt werden, die Firmeware muss dafür aber selber angepasst werden. Achtung, bei Boards ohne PSRAM dürfen maximal 50 Kanäle definiert werden.

## Firmware

Eine vorkomplierte Firmware ist hier ... zu finden. ZIP Datei herunterladen, entpacken und der Anleitung im Readme folgen.

