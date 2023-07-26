
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

    // Start loop with 2, because device 1 is the bridge
    int devices = BRI_ChannelCount;
    int start = 2;
    int blockNumber = 0;
    for (int device = start; device < start + devices; device++)
    {
      uint16_t parameterOffset = BRI_ParamBlockOffset + blockNumber * BRI_ParamBlockSize;
      uint16_t goOffset = BRI_KoOffset + blockNumber * BRI_KoBlockSize;
      blockNumber++;

      uint8_t deviceType = knx.paramByte(parameterOffset + BRI_CHDeviceType);
#ifdef KDEBUG_min
      SERIAL_PORT.print("Device ");
      SERIAL_PORT.print(device - start + 1);
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
          switchInterfaces->push_back(new HomeKitSwitch(device));
        if (bridge->mode & Mode::HueBridgeEmulation)
          switchInterfaces->push_back(new HueSwitch(hueBridge));
        new KnxChannelSwitch(switchInterfaces, goOffset, parameterOffset);
        break;
      }
      case 2:
      {
#ifdef KDEBUG_min
        SERIAL_PORT.println("Dimmer");
#endif
        std::list<IDimmerInterface *> *dimmerInterfaces = new std::list<IDimmerInterface *>();
        if (bridge->mode & Mode::Homekit)
          dimmerInterfaces->push_back(new HomeKitDimmer(device));
        if (bridge->mode & Mode::HueBridgeEmulation)
          dimmerInterfaces->push_back(new HueDimmer(hueBridge));
        new KnxChannelDimmer(dimmerInterfaces, goOffset, parameterOffset);
        break;
      }
      default:
      {
#ifdef KDEBUG_min
        SERIAL_PORT.println("Inactive");
#endif
        new KnxChannelBase(goOffset, parameterOffset);
        break;
      }
      }
    }
  }
}


