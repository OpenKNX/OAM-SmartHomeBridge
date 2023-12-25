#include "OpenKNX.h"
#include "Logic.h"
#include "hardware.h"
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
    knx.platform().knxUartPins(KNX_UART_RX_PIN, KNX_UART_TX_PIN);
    const uint8_t firmwareRevision = 1;
    openknx.init(firmwareRevision);

// GPIO1 is used for serial TX, special handling needed to turn of Serial
#if (USE_PROG_LED_ON_SERIAL_TX == 1)
    openknx.progLed.init(0, 0);
    progLedOff();
    knx.setProgLedOffCallback(progLedOff);
    knx.setProgLedOnCallback(progLedOn);
#endif

    openknx.addModule(1, new Logic());
    openknx.addModule(2, new SmartHomeBridgeModule());
    openknx.addModule(3, new OTAUpdateModule());

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