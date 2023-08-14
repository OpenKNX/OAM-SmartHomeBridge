#include <knx.h>
#include "hardware.h"
#include <WiFi.h>
#include "KnxBridge.h"
#include "KnxBridgeDevice.h"
#include "KnxChannelSwitch.h"
#include "KnxChannelDimmer.h"
#include "KnxChannelRolladen.h"
#include "KnxChannelJalousie.h"
#include "KnxChannelThermostat.h"
#include "KnxChannelDisplay.h"

#include "HomeKitBridge.h"
#include "HomeKitSwitch.h"
#include "HomeKitDimmer.h"
#include "HomeKitRolladen.h"
#include "HomeKitJalousie.h"
#include "HomeKitThermostat.h"
#include "HomeKitDisplay.h"

#include "HueBridge.h"
#include "HueSwitch.h"
#include "HueDimmer.h"
#include "Bridge.h"

void KnxBridge::setup()
{
    KnxBridgeDevice *bridge = new KnxBridgeDevice();
    _components.push_back(bridge);

    std::list<IBridge *> *bridgeInterfaces = new std::list<IBridge *>();
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
      int homekitAID = _channelIndex + 2; // Homekit bridge has AID0
      uint8_t deviceType = ParamBRI_CHDeviceType;

      Serial.print("Device ");
      Serial.print(_channelIndex + 1);
      Serial.print(" AID ");
      Serial.print(homekitAID);
      Serial.print(": ");

      switch (deviceType)
      {
      case 1:
      {
        Serial.println("Switch");
        std::list<ISwitchBridge *> *switchBridges = new std::list<ISwitchBridge *>();
        if (bridge->mode & Mode::Homekit)
          switchBridges->push_back(new HomeKitSwitch(homekitAID));
        if (bridge->mode & Mode::HueBridgeEmulation)
          switchBridges->push_back(new HueSwitch(hueBridge));
        _components.push_back(new KnxChannelSwitch(switchBridges, _channelIndex));
        break;
      }
      case 2:
      {
        Serial.println("Dimmer");
        std::list<IDimmerBridge *> *dimmerBridges = new std::list<IDimmerBridge *>();
        if (bridge->mode & Mode::Homekit)
          dimmerBridges->push_back(new HomeKitDimmer(homekitAID));
        if (bridge->mode & Mode::HueBridgeEmulation)
          dimmerBridges->push_back(new HueDimmer(hueBridge));
        _components.push_back(new KnxChannelDimmer(dimmerBridges, _channelIndex));
        break;
      }
      case 3:
      {
        Serial.println("Jalousien");
        std::list<IJalousieBridge *> *jalousieBridges = new std::list<IJalousieBridge *>();
        if (bridge->mode & Mode::Homekit)
          jalousieBridges->push_back(new HomeKitJalousie(homekitAID));
        //if (bridge->mode & Mode::HueBridgeEmulation)
        //   jalousiendInterfaces->push_back(new HueDimmer(hueBridge));
        _components.push_back(new KnxChannelJalousie(jalousieBridges, _channelIndex));
        break;
      }
      case 4:
      {
        Serial.println("Rolladen/Markise");
        std::list<IRolladenBridge *> *rolladenBridges = new std::list<IRolladenBridge *>();
        if (bridge->mode & Mode::Homekit)
          rolladenBridges->push_back(new HomeKitRolladen(homekitAID));
        // if (bridge->mode & Mode::HueBridgeEmulation)
        //   rolladenBridge->push_back(new HueDimmer(hueBridge));
        _components.push_back(new KnxChannelRolladen(rolladenBridges, _channelIndex));
        break;
      }
      case 5:
      {
        Serial.println("Thermostat");
        std::list<IThermostatBridge *> *thermostatBridges = new std::list<IThermostatBridge *>();
        if (bridge->mode & Mode::Homekit)
          thermostatBridges->push_back(new HomeKitThermostat(homekitAID));   
        _components.push_back(new KnxChannelThermostat(thermostatBridges, _channelIndex));
        break;
      }
      case 6:
      {
        Serial.println("Display");
        std::list<IDisplayBridge *> *displayBridges = new std::list<IDisplayBridge *>();
        if (bridge->mode & Mode::Homekit)
          displayBridges->push_back(new HomeKitDisplay(homekitAID));   
        _components.push_back(new KnxChannelDisplay(displayBridges, _channelIndex));
        break;
      }
      default:
      {
        Serial.println("Inactive");
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