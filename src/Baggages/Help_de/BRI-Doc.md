### Smart Home Bridge

Die Smart Home Bridge stellt KNX Geräte wie Lampen, Dimmer, Lüfter usw. als Geräte für andere Systeme zur Verfügung. 
Um die Bridge verwenden zu können, muss unter den Basiseinstellungen das WLAN richtig konfiguriert werden. Der WLAN-Verbindiungsstatus kann über einen KO abgefragt werden.

Folgende Smart Home Systeme werden unterstüzt:

### Apple HomeKit

Über Apple HomeKit können die KNX Geräte gesteuert werden. Es werden bis zu 149 Geräte unterstützt. Das Limit ergibt sich aus eine Beschränkung der Anzahl der Accessories bei einem HomeKit Gerät mit maximal 150. Eines davon repräsentiert das Bridge Gerät.

Nachdem die Bridge über ETS programmiert wurde, kann sie mit Apple Homekit verbunden:

- "Gerät hinzufügen" wählen
- "Weitere Optionen..." wählen
- Nun sollte die Bridge sichtbar sein. Den Kopplungscode der in ETS eingestellt wurde (Standardwert 46637726) eingeben und die Meldung das es sich um ein nicht zertifiziertes Gerät handelt bestätigen.
- Danach den Setup-Wizard für alle Geräte durchführen.

Sollte nachträglich eine Gerätetype sich ändern, z.B. eine Lampe wird zu einer Jalousie, oder eine Untertype ändert sich z.B. ein CO2 Sensor wird zu einem Kontakt, muss zuerst das Gerät deaktiviert werden. Danach die in der Home App warten bis das Gerät verschwunden ist und danach kann das Gerät wieder aktiviert werden.

### Philips Hue Emulation für Amazon Alexa

Die Philips Hue Emulation funktiontioniert nur für Amazon Echo Geräte, d.h. in der Hue App können die Geräte nicht verwendet werden.
Philips Hue unterstützt nur Lampen, für einige Gerätetypen gibt es aber die Möglichkeit diese als Lampen in Alexa anzeigen zu lassen.

Achtung: Die Anbindung wird instabil wenn zu viele Geräte definiert sind. Es empfiehlt sich nicht mehr als etwa 30 Geräte für Philips Hue zu nutzen. Die Anzahl der Geräte die für Homekit verwendet werden spielt dabei keine Rolle.
