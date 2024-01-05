#include "OpenKNX.h"
#include "Logic.h"
#include "hardware.h"
//#include "NetworkModule.h"
#include "SmartHomeBridgeModule.h"
#include "OTAUpdateModule.h"

#if PROG_LED_PIN2
#ifndef PROG_LED_PIN2_ACTIVE_ON
#define PROG_LED_PIN2_ACTIVE_ON HIGH
#endif
#endif

#if PROG_LED_PIN3
#ifndef PROG_LED_PIN3_ACTIVE_ON
#define PROG_LED_PIN3_ACTIVE_ON HIGH
#endif
#endif

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

OpenKNX::Led* led2 = nullptr;
OpenKNX::Led* led3 = nullptr;

void setup()
{ 
    const uint8_t firmwareRevision = 1;
#ifdef PROG_LED_PIN2
    led2 = new OpenKNX::Led();
    led2->init(PROG_LED_PIN2, PROG_LED_PIN2_ACTIVE_ON);
    led2->pulsing();
#endif
#ifdef PROG_LED_PIN3
    led3 = new OpenKNX::Led();
    led3->init(PROG_LED_PIN3, PROG_LED_PIN2_ACTIVE_ON);
    led3->pulsing();
#endif
    openknx.init(firmwareRevision);
// GPIO1 is used for serial TX, special handling needed to turn of Serial
#if (USE_PROG_LED_ON_SERIAL_TX == 1)
    progLedOff();
    knx.setProgLedOffCallback(progLedOff);
    knx.setProgLedOnCallback(progLedOn);
#endif

    openknx.addModule(1, openknxLogic);
    openknx.addModule(2, openknxSmartHomeBridgeModule);
    openknx.addModule(3, openknxOTAUpdateModule);
   // openknx.addModule(4, openknxNetwork);

    KNX_SERIAL.setRxBufferSize(1024);
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
bool firstLoop = true;
bool lastProgMode = false;
void loop()
{
#ifdef PROG_BUTTON_PIN2
  bool button2Pressed = digitalRead(PROG_BUTTON_PIN2);
  if (PROG_BUTTON_PIN2_INTERRUPT_ON == FALLING)
    button2Pressed =! button2Pressed;
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
    button3Pressed =! button3Pressed;
  if (button3Pressed != lastButton3Pressed)
  {
    lastButton3Pressed = button3Pressed;
    if (button3Pressed)
      knx.toggleProgMode();
  }
#endif
  if (lastProgMode != knx.progMode() || firstLoop)
  {
    lastProgMode = knx.progMode();
    if (lastProgMode)
    {
      if (led2 != nullptr)
        led2->on();
      if (led3 != nullptr)
        led3->on();
    }
    else
     {
      if (led2 != nullptr)
        led2->off();
      if (led3 != nullptr)
        led3->off();
    }
  }
  if (led2 != nullptr)
    led2->loop();
  if (led3 != nullptr)
    led3->loop();
  openknx.loop(); 
  firstLoop = false;
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