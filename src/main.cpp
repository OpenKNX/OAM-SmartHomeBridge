#include "OpenKNX.h"
#include "Logic.h"
#include "hardware.h"
//#include "NetworkModule.h"
#include "SmartHomeBridgeModule.h"
#include "OTAUpdateModule.h"

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


void setup()
{ 
    const uint8_t firmwareRevision = 1;
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
}

void loop()
{
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