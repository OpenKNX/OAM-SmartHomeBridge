
#include <knx.h>
#include "hardware.h"
#include <WiFi.h>
#include "KnxBridgeDevice.h"
#include "KnxChannelSwitch.h"
#include "KnxChannelDimmer.h"

#include "HomeKitBridge.h"
#include "HomeKitSwitch.h"
#include "HomeKitDimmer.h"

#include "HueBridge.h"
#include "HueSwitch.h"
#include "HueDimmer.h"
#include "Bridge.h"




void processKoCallback(GroupObject &iKo)
{
  Component::receiveAll(iKo);
}

void appSetup()
{
  if (knx.configured())
  {
    GroupObject::classCallback(processKoCallback);
  #ifdef KDEBUG_min
    SERIAL_PORT.println("Initialize group objects and sensors");
  #endif

  
    //   startTimeInMilliseconds = knx.paramInt(parameterAddress) * 1000;
    //   Serial.print("Start time ");
    //   Serial.println(startTimeInMilliseconds);
  
    KnxBridgeDevice *bridge = new KnxBridgeDevice();

    std::list<IBridgeInterface *> *bridgeInterfaces = new std::list<IBridgeInterface *>();
    HueBridge *hueBridge = NULL;
    if (bridge->mode & Mode::Homekit)
      bridgeInterfaces->push_back(new HomeKitBridge());

    if (bridge->mode & Mode::HueBridgeEmulation)
    {
      hueBridge = new HueBridge();
      bridgeInterfaces->push_back(hueBridge);
    }

    bridge->initialize(bridgeInterfaces);


    for (uint8_t _channelIndex = 0; _channelIndex < BRI_ChannelCount; _channelIndex++)
    {
    
      uint8_t deviceType = ParamBRI_CHDeviceType;
#ifdef KDEBUG_min
      SERIAL_PORT.print("Device ");
      SERIAL_PORT.print(_channelIndex + 1);
      SERIAL_PORT.print(": ");
#endif
      switch (deviceType)
      {
      case 1:
      {
#ifdef KDEBUG_min
        SERIAL_PORT.println("Switch");
#endif
        std::list<ISwitchInterface *> *switchInterfaces = new std::list<ISwitchInterface *>();
        if (bridge->mode & Mode::Homekit)
          switchInterfaces->push_back(new HomeKitSwitch(_channelIndex));
        if (bridge->mode & Mode::HueBridgeEmulation)
          switchInterfaces->push_back(new HueSwitch(hueBridge));
        new KnxChannelSwitch(switchInterfaces, _channelIndex);
        break;
      }
      case 2:
      {
#ifdef KDEBUG_min
        SERIAL_PORT.println("Dimmer");
#endif
        std::list<IDimmerInterface *> *dimmerInterfaces = new std::list<IDimmerInterface *>();
        if (bridge->mode & Mode::Homekit)
          dimmerInterfaces->push_back(new HomeKitDimmer(_channelIndex));
        if (bridge->mode & Mode::HueBridgeEmulation)
          dimmerInterfaces->push_back(new HueDimmer(hueBridge));
        new KnxChannelDimmer(dimmerInterfaces, _channelIndex);
        break;
      }
      default:
      {
#ifdef KDEBUG_min
        SERIAL_PORT.println("Inactive");
#endif
        new KnxChannelBase(_channelIndex);
        break;
      }
      }
    }
  }
}


