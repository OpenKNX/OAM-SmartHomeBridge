
#include <knx.h>
#include "hardware.h"
#include <WiFi.h>
#include "KnxBridge.h"
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


void KnxBridge::setup()
{
    KnxBridgeDevice *bridge = new KnxBridgeDevice();
    _components.push_back(bridge);

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
        _components.push_back(new KnxChannelSwitch(switchInterfaces, _channelIndex));
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
        _components.push_back(new KnxChannelDimmer(dimmerInterfaces, _channelIndex));
        break;
      }
      default:
      {
#ifdef KDEBUG_min
        SERIAL_PORT.println("Inactive");
#endif
        break;
      }
      }
    }
}
void KnxBridge::loop()
{
  unsigned long now = millis();
  if (now == 0)
    now = 1; // Never use 0, it's used for marker

  for (std::list<Component*>::iterator it=_components.begin(); it != _components.end(); ++it)
  {
        (*it)->loop(now, _initalize);
  }
  _initalize = false;

}
void KnxBridge::processInputKo(GroupObject &ko)
{
    for (std::list<Component*>::iterator it=_components.begin(); it != _components.end(); ++it)
    {
        (*it)->received(ko);
    }
}


