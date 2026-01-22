#include "OpenKNX.h"
#include "DNSServer.h"
#include "NetworkModule.h"
#include "FileTransferModule.h"
#include "Logic.h"
#include "SmartHomeBridgeModule.h"
#include "FunctionBlocksModule.h"
#include "pins_arduino.h"

#ifdef PROG_BUTTON_PIN2
#ifndef PROG_BUTTON_PIN2_INTERRUPT_ON
#define PROG_BUTTON_PIN2_INTERRUPT_ON RISING
#endif
#endif

#ifdef PROG_BUTTON_PIN3
#ifndef PROG_BUTTON_PIN3_INTERRUPT_ON
#define PROG_BUTTON_PIN3_INTERRUPT_ON RISING
#endif
#endif

// GPIO1 is used for serial TX, use special handling to turn off the Serial
#if (USE_PROG_LED_ON_SERIAL_TX == 1)

void progLedOn()
{
  Serial.println("Prog LED ON");
  Serial.flush(true);
  Serial.end(true);
  pinMode(USE_PROG_LED_ON_SERIAL_TX, OUTPUT);
  digitalWrite(USE_PROG_LED_ON_SERIAL_TX, PROG_LED_PIN_ACTIVE_ON);
}

void progLedOff()
{
  digitalWrite(USE_PROG_LED_ON_SERIAL_TX, PROG_LED_PIN_ACTIVE_ON);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Prog LED Off");
}

#endif
 // Adafruit_NeoPixel pixels(1, OPENKNX_SERIALLED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  openknx.init();
 
// GPIO1 is used for serial TX, special handling needed to turn of Serial
#if (USE_PROG_LED_ON_SERIAL_TX == 1)
  progLedOff();
  knx.setProgLedOffCallback(progLedOff);
  knx.setProgLedOnCallback(progLedOn);
#endif
#ifdef WLAN_WifiSSID
  openknx.addModule(3, openknxWLANModule);
#endif
#ifdef NET_ModuleVersion
  openknx.addModule(2, openknxNetwork);
#endif
  openknx.addModule(1, openknxLogic);

#ifdef ARDUINO_ARCH_RP2040
  openknx.addModule(5, openknxUsbExchangeModule);
  openknx.addModule(6, openknxFileTransferModule);
#endif
  openknx.addModule(7, openknxSmartHomeBridgeModule);
  openknx.addModule(8, openknxFunctionBlocksModule);
  openknx.setup();

#ifdef PROG_BUTTON_PIN2
  if (PROG_BUTTON_PIN2_INTERRUPT_ON == FALLING)
    pinMode(PROG_BUTTON_PIN2, INPUT_PULLUP);
  else
    pinMode(PROG_BUTTON_PIN2, INPUT_PULLDOWN);
#endif
#ifdef PROG_BUTTON_PIN3
  if (PROG_BUTTON_PIN3_INTERRUPT_ON == FALLING)
    pinMode(PROG_BUTTON_PIN3, INPUT_PULLUP);
  else
    pinMode(PROG_BUTTON_PIN3, INPUT_PULLDOWN);
#endif
}
bool lastButton2Pressed = false;
bool lastButton3Pressed = false;
bool lastWifiConntected = false;

void loop()
{

#ifdef PROG_BUTTON_PIN2
  bool button2Pressed = digitalRead(PROG_BUTTON_PIN2);
  if (PROG_BUTTON_PIN2_INTERRUPT_ON == FALLING)
    button2Pressed = !button2Pressed;
  if (button2Pressed != lastButton2Pressed)
  {
    lastButton2Pressed = button2Pressed;
    if (button2Pressed)
      knx.toggleProgMode();
  }
#endif
#ifdef PROG_BUTTON_PIN3
  bool button3Pressed = digitalRead(PROG_BUTTON_PIN3);
  if (PROG_BUTTON_PIN3_INTERRUPT_ON == FALLING)
    button3Pressed = !button3Pressed;
  if (button3Pressed != lastButton3Pressed)
  {
    lastButton3Pressed = button3Pressed;
    if (button3Pressed)
      knx.toggleProgMode();
  }
#endif
  openknx.loop();
}

#ifdef OPENKNX_DUALCORE
void setup1()
{
  openknx.setup1();
}
void loop1()
{
  openknx.loop1();
}
#endif