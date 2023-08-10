#include "OpenKNX.h"
#include "Logic.h"

#include "KnxBridge.h"



  
// class DevBoardLed : public OpenKNX::LedHardware
// {
// public:
//   DevBoardLed() : OpenKNX::LedHardware(LED_BUILTIN, LOW)
//   {
//   }

//   virtual void init()
//   {
//     if (_pin == 1) // GPIO1 is used for serial TX, do nothing here
//       return;
//     LedHardware::init();
//   }

//   virtual void write(uint8_t brightness)
//   {
// #ifdef ARDUINO_ARCH_ESP32 
//     if (_pin == 1 && brightness > 0) // GPIO1 is used for serial TX, to turn on the LED, the serial communication must be stopped
//     {
//         Serial.println("Devboard LED On");        
//         Serial.flush(true);
//         Serial.end(true);
//      //   pinMode(_pin, OUTPUT);
//     }
// #endif
//     //LedHardware::write(brightness);

//         if (brightness == 255)
//             digitalWrite(_pin, _activeOn == HIGH ? true : false);
//         else if (brightness == 0)
//             digitalWrite(_pin, _activeOn == HIGH ? false : true);
//         else
//             analogWrite(_pin, _activeOn == HIGH ? brightness : (255 - brightness));
// #ifdef ARDUINO_ARCH_ESP32
//     if (_pin == 1 && brightness == 0) // GPIO1 is used for serial TX, to turn on the LED, the serial communication must be stopped
//     {
//         Serial.begin(115200);
//         Serial.println();
//         Serial.println("Devboard LED Off");
//     }
// #endif
//   }
// };



void setup()
{
    const uint8_t firmwareRevision = 1;
    openknx.init(firmwareRevision);
    Serial.print(KNX_FLASH_SIZE);
    openknx.addModule(1, new Logic());

    openknx.addModule(2, new KnxBridge());
#ifdef ARDUINO_ARCH_RP2040
    openknx.addModule(9, new UpdaterModule());
#endif
    openknx.setup();
  
}


void loop()
{
  openknx.loop();
  
}


