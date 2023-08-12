#include <knx.h>
#include "hardware.h"
#include <WiFi.h>
#include "KnxBridge.h"
#include "KnxBridgeDevice.h"
#include "KnxChannelSwitch.h"
#include "KnxChannelDimmer.h"
#include "KnxChannelRolladen.h"
#include "KnxChannelJalousie.h"

#include "HomeKitBridge.h"
#include "HomeKitSwitch.h"
#include "HomeKitDimmer.h"
#include "HomeKitRolladen.h"
#include "HomeKitJalousie.h"

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
        std::list<ISwitchBridge *> *switchBridges = new std::list<ISwitchBridge *>();
        if (bridge->mode & Mode::Homekit)
          switchBridges->push_back(new HomeKitSwitch(_channelIndex));
        if (bridge->mode & Mode::HueBridgeEmulation)
          switchBridges->push_back(new HueSwitch(hueBridge));
        _components.push_back(new KnxChannelSwitch(switchBridges, _channelIndex));
        break;
      }
      case 2:
      {
#ifdef KDEBUG_min
        SERIAL_PORT.println("Dimmer");
#endif
        std::list<IDimmerBridge *> *dimmerBridges = new std::list<IDimmerBridge *>();
        if (bridge->mode & Mode::Homekit)
          dimmerBridges->push_back(new HomeKitDimmer(_channelIndex));
        if (bridge->mode & Mode::HueBridgeEmulation)
          dimmerBridges->push_back(new HueDimmer(hueBridge));
        _components.push_back(new KnxChannelDimmer(dimmerBridges, _channelIndex));
        break;
      }
      case 3:
      {
#ifdef KDEBUG_min
        SERIAL_PORT.println("Jalousien");
#endif
        std::list<IJalousieBridge *> *jalousieBridges = new std::list<IJalousieBridge *>();
        if (bridge->mode & Mode::Homekit)
          jalousieBridges->push_back(new HomeKitJalousie(_channelIndex));
        //if (bridge->mode & Mode::HueBridgeEmulation)
        //   jalousiendInterfaces->push_back(new HueDimmer(hueBridge));
        _components.push_back(new KnxChannelJalousie(jalousieBridges, _channelIndex));
        break;
      }
      case 4:
      {
#ifdef KDEBUG_min
        SERIAL_PORT.println("Jalousien");
#endif
        std::list<IRolladenBridge *> *rolladenBridge = new std::list<IRolladenBridge *>();
        if (bridge->mode & Mode::Homekit)
          rolladenBridge->push_back(new HomeKitRolladen(_channelIndex));
        // if (bridge->mode & Mode::HueBridgeEmulation)
        //   rolladenBridge->push_back(new HueDimmer(hueBridge));
        _components.push_back(new KnxChannelRolladen(rolladenBridge, _channelIndex));
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