# OpenKNX Smart Home Bridge

Die OpenKNX Smart Home Bridge erlaubt KNX Geräte über Apple Home und Amazon Alexa zu steuern.

## Features

- Homekit Unterstützung 
- Hue Emulation für Amazon Alex für: Schaltbare und dimmbare Lampen
- Hue Emulation für Rollanden und Jalousien, dargestellt als dimmbare Lampen
- SW-Updates können über WLAN (OTA) eigespielt werden

### Unterstützte Geräte

- Schalten
- Steckdose
- Lampe (Schaltbar, Dimmbar, RGB)
- Jalousie (In Alexa als dimmbare Lampe dargestellbar)
- Rolladen (In Alexa als dimmbare Lampe dargestellbar)
- Markise (In Alexa als dimmbare Lampe dargestellbar)
- Thermostat (Nur Homekit)
- Anzeige (Temperatur, Luftfeuchtigkeit, Helligkeit) (Nur Homekit)
- Kontakt (Nur Homekit)
- Bewegungsmelder (Nur Homekit)
- Präsensmelder (Nur Homekit)
- Leckmelder (Nur Homekit)
- Rauchmelder (Nur Homekit)
- Kohlendioxidmelder (Nur Homekit)
- Kohlenmonoxidmelder (Nur Homekit)
- Lüfter (In Alexa als schaltbare Lampe dargestellbar)
- Fenster (Mit oder ohne Motorantrieb) (Nur Homekit)
- Tür (Mit oder ohne Motorantrieb) (Nur Homekit)
- Garagentor (Mit oder ohne Motorantrieb) (Nur Homekit)

## Anwenderdokumentation

Die Anwenderdokumentation ist [hier](./doc/Applikationsbeschreibung.md) zu finden.

## Firmware

Eine vorkomplierte Firmware ist [hier](https://github.com/OpenKNX/OAM-SmartHomeBridge/releases) zu finden. ZIP Datei herunterladen, entpacken und der Anleitung im Readme folgen.

## Hardware

Als Hardware kann jede OpenKNX oder OpenKNX-Ready Hardware mit LAN oder WLAN verwendet werden.
Die vorkompilierte Firmware unterstützt:

- [REG1-LAN-TP-Base](https://github.com/OpenKNX/OpenKNX/wiki/REG1-LAN-TP-Base)
- [Adafruit ESP32 Feather V2](https://github.com/OpenKNX/OpenKNX/wiki/Adafruit-ESP32-Feather-V2)

### Optional bei Adafruit ESP32 Feather V2: Zusätzlicher Prog Taster und LED

An Pin GPIO 7 (RX) und/oder GPIO 20 (am Stecker) kann jeweils ein zusätzlicher Taster angeschlossen werden. Dieser muss gegen GND schalten.

An PIN GPIO 8 (TX) und/oder GPIO 22 (am Stecker) kann mit einem 100 Ohm Wiederstand eine LED (Anode) angeschlossen werden. Die Kathode mit GND verbinden.


## Lizenz

Diese Software steht unter der [GNU GPL v3](LICENSE).