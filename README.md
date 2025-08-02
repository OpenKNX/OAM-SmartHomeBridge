# OpenKNX Smart Home Bridge

Die OpenKNX Smart Home Bridge erlaubt KNX Geräte über Apple Home und Amazon Alexa zu steuern.

## Features

- Homekit Unterstützung 
- Hue Emulation für Amazon Alex für: Schaltbare und dimmbare Lampen
- Hue Emulation für Rolllanden und Jalousien, dargestellt als dimmbare Lampen
- SW-Updates können über WLAN (OTA) eigespielt werden

### Unterstützte Geräte

| Gerät                                              | Homekit | Alexa über Hue     |
|----------------------------------------------------|---------|--------------------|
| Ein- Auschaltbares Gerät                           | Ja      | Ja                 |
| Steckdose                                          | Ja      | Ja                 |
| Lampe (Schaltbar, Dimmbar, RGB)                    | Ja      | Ja                 |
| Jalousie                                           | Ja      | als dimmbare Lampe |
| Rollladen                                          | Ja      | als dimmbare Lampe |
| Markise                                            | Ja      | als dimmbare Lampe |
| Thermostat                                         | Ja      | -                  |
| Anzeige (Temperatur, Luftfeuchtigkeit, Helligkeit) | Ja      | -                  |
| Alarm (Kontakt, Bewegungsmelder, Leckmelder, Rauchmelder, Kohlendioxidmelder, Kohlenmonoxidmelder) | Ja      | -                  |
| Lüfter                                             | Ja      | als schaltbare Lampe |
| Fenster (Mit und ohne Motorantrieb)                | Ja      | -                    |
| Tür (Mit und ohne Motorantrieb)                    | Ja      | -                    |
| Garagentor (Mit und ohne Motorantrieb)             | Ja      | -                    |
| Szene                                              | Ja      | als schaltbare Lampe |

## Anwenderdokumentation

Die Anwenderdokumentation ist [hier](./doc/Applikationsbeschreibung.md) zu finden.


## Übertragung des HomeKit Pairings von einem Gerät zu einem anderen

Wichtig bei den Prozess ist, dass immer nur eines der beiden Geräte eingeschalten sein darf.
Die beiden Geräte benötigen die gleiche Gerätekonfiguration in der ETS.
Die Firmware darf sich unterscheiden.
Weitere Details zu dem Prozess sind auf dieser Seite zu finden: https://github.com/HomeSpan/HomeSpan/blob/master/docs/Cloning.md#cloning-a-homespan-device

**Altes Gerät**
- Das Gerät vom KNX Bus nehmen und mit USB mit dem Computer verbinden.
- Mit einem Terminal auf die OpenKNX-Console verbinden
- Das Gerät mit dem Netzwerk oder Wifi verbinden
- Befehl 'hk' + ENTER eingeben
  Damit wird die Homekit Console gestartet
- Befehl 'P' + ENTER eingeben
  Auf der Console erscheint nun eine Block ähnlich diesem Beispiel:
```
*** Pairing Data used for Cloning another Device

Accessory data:  M0M6RTM6Mjk6MzU6QTA6NzY8ZEvYrUkwU5eCoS2MnfhyqT834xZhBjn0vIgoLOlSrP697eRZou0l0Ks60Rdmsa6uSjnwrBkwFkM1+ZaRMQry1r3t5Fmi7SXQqzrRF2axrq5KOfCsGTAWQzX5lpExCvLU=
Controller data: AQE2MTQ1N0E0RC1FNjUzLTA0M8gtQTYxQy1FRTg1RjhDOUVCQjl3I0SvCSPzuiEvBh2SzYzs8xGrs+URmK2TkalbNQAYTw==

*** End Pairing Data
```  

Zeilen mit "Controller data" kann es auch mehrere geben.

- Diesen Abschnitt in die Zwischenablage kopieren und irgendwo sichern
- Altes Gerät von der Stromversorgung (bzw. KNX Bus) trennen

**Neues Gerät**

- Gerät in Betrieb nehmen und die gleiche Geräte Konfiguration über die ETS einspielen.
- Gerät vom KNX Bus nehmen und USB Kabel mit Computer verbinden
- Mit einem Terminal mit der OpenKNX Console verbinden
- Gerät mit Netwerk oder Wifi verbinden.
- Befehl 'hk' + ENTER eingeben
  Damit wird die Homekit Console gestartet#
- Befehl 'C' + ENTER eingeben
- Es werden nun die "Accessory data" abgefragt. 
- Die Accessory Daten einfügen (In der Console wird dabei nichts angezeigt) und mit ENTER bestätigen (Nun erscheine 6 Hex Zahlen)
- Es werden nun die "Controller data" abgefragt
- Die Controller Daten einfügen (In der Console wird dabei nichts angezeigt) und mit ENTER bestätigen (Nun erscheine eine Längere Zeichenkette (GUID))
- Sind weitere Controller Daten beim Export exportiert worden, den Vorgang wiederholen
- Sind alle Controller Daten eingefügt worden, die Abfrage nach weiteren Controller Daten einfach mit ENTER bestätigen
- Es erfolgt eine Abrage "Saved Cloned Pairing Data (y/n)", diese mit y und ENTER bestätigen
- Prüfen ob HomeKit funktioniert

**Altes Gerät wiederverwenden**

Hat die Übertragung geklappt und HomeKit funktioniert, kann das alte Gerät nun für andere Zwecke genutzt werden.
Dazu muss sicher gestellt werden, dass es nicht gleichzeitig mit dem neuen Gerät in Betrieb ist.

- Beim alten Gerät die Stromversorgung (bzw. den KNX Bus) trennen.
- Altes Gerät mit USB verbinden und die Console verbinden
- Altes Gerät mit Netzwerk oder Wifi verbinden
- Befehl 'hk' eingeben
- Mit '' Factory reset auslösen
- Nun kann das alte Gerät beliebig wieder verwendet werden
- Das neue Gerät kann nun ebenfalls wieder in Betrieb genommen werden.

## Hue Pairing von einem Gerät auf ein neues übertragen

Leider ist eine Übertragung nicht möglich. In der Alexa App die Smart Home Geräte entfernen und anschließen das neue Gerät über die Hue Suche einbinden.

## Firmware

Eine vorkomplierte Firmware ist [hier](https://github.com/OpenKNX/OAM-SmartHomeBridge/releases) zu finden. ZIP Datei herunterladen, entpacken und der Anleitung im Readme folgen.

## Hardware

Als Hardware kann jede OpenKNX oder OpenKNX-Ready Hardware mit LAN oder WLAN verwendet werden.
Die vorkompilierte Firmware unterstützt:

- [REG1-LAN-TP-Base](http://device.openknx.de/REG1-LAN-TP-Base)
- [Adafruit ESP32 Feather V2](https://github.com/OpenKNX/OpenKNX/wiki/Adafruit-ESP32-Feather-V2)

### Optional bei Adafruit ESP32 Feather V2: Zusätzlicher Prog Taster und LED

An Pin GPIO 7 (RX) und/oder GPIO 20 (am Stecker) kann jeweils ein zusätzlicher Taster angeschlossen werden. Dieser muss gegen GND schalten.

An PIN GPIO 8 (TX) und/oder GPIO 22 (am Stecker) kann mit einem 100 Ohm Wiederstand eine LED (Anode) angeschlossen werden. Die Kathode mit GND verbinden.


## Lizenz

Diese Software steht unter der [GNU GPL v3](LICENSE).
