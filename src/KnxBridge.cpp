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
#include "KnxChannelSensor.h"

#include "HomeKitBridge.h"
#include "HomeKitSwitch.h"
#include "HomeKitDimmer.h"
#include "HomeKitRolladen.h"
#include "HomeKitJalousie.h"
#include "HomeKitThermostat.h"
#include "HomeKitDisplay.h"
#include "HomeKitSensor.h"

#include "HueBridge.h"
#include "HueSwitch.h"
#include "HueDimmer.h"
#include "HueRolladen.h"
#include "HueJalousie.h"
#include "HueDimmer.h"
#include "Bridge.h"

void KnxBridge::setup()
{
    Mode mode = (Mode) ParamBRI_Modus;
    KnxBridgeDevice *bridge = new KnxBridgeDevice();
    _components.push_back(bridge);

    std::list<IBridge *> *bridgeInterfaces = new std::list<IBridge *>();
    HueBridge *hueBridge = NULL;
    if (mode & Mode::Homekit)
      bridgeInterfaces->push_back(new HomeKitBridge());

    if (mode & Mode::HueBridgeEmulation)
    {
        Serial.println("HueBridge");
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
      case 0:
      {
        Serial.println("Inactive");
        break;
      }
      case 1:
      {
        Serial.println("Switch");
        std::list<ISwitchBridge *> *switchBridges = new std::list<ISwitchBridge *>();
        if (mode & Mode::Homekit)
          switchBridges->push_back(new HomeKitSwitch(homekitAID));
        if (mode & Mode::HueBridgeEmulation)
          switchBridges->push_back(new HueSwitch(hueBridge));
        _components.push_back(new KnxChannelSwitch(switchBridges, _channelIndex));
        break;
      }
      case 2:
      {
        Serial.println("Dimmer");
        std::list<IDimmerBridge *> *dimmerBridges = new std::list<IDimmerBridge *>();
        if (mode & Mode::Homekit)
          dimmerBridges->push_back(new HomeKitDimmer(homekitAID));
        if (mode & Mode::HueBridgeEmulation)
          dimmerBridges->push_back(new HueDimmer(hueBridge));
        _components.push_back(new KnxChannelDimmer(dimmerBridges, _channelIndex));
        break;
      }
      case 3:
      {
        Serial.println("Jalousien");
        std::list<IJalousieBridge *> *jalousieBridges = new std::list<IJalousieBridge *>();
        if (mode & Mode::Homekit)
          jalousieBridges->push_back(new HomeKitJalousie(homekitAID));
        if (mode & Mode::HueBridgeEmulation && BRI_CHJalousieHueEmulation)
          jalousieBridges->push_back(new HueJalousie(hueBridge));
        _components.push_back(new KnxChannelJalousie(jalousieBridges, _channelIndex));
        break;
      }
      case 4:
      {
        Serial.println("Rolladen/Markise");
        std::list<IRolladenBridge *> *rolladenBridges = new std::list<IRolladenBridge *>();
        if (mode & Mode::Homekit)
          rolladenBridges->push_back(new HomeKitRolladen(homekitAID));
        if (mode & Mode::HueBridgeEmulation && BRI_CHRolladenHueEmulation)
          rolladenBridges->push_back(new HueRolladen(hueBridge));
        _components.push_back(new KnxChannelRolladen(rolladenBridges, _channelIndex));
        break;
      }
      case 5:
      {
        Serial.println("Thermostat");
        std::list<IThermostatBridge *> *thermostatBridges = new std::list<IThermostatBridge *>();
        if (mode & Mode::Homekit)
          thermostatBridges->push_back(new HomeKitThermostat(homekitAID));   
        _components.push_back(new KnxChannelThermostat(thermostatBridges, _channelIndex));
        break;
      }
      case 6:
      {
        Serial.println("Display");
        std::list<IDisplayBridge *> *displayBridges = new std::list<IDisplayBridge *>();
        if (mode & Mode::Homekit)
          displayBridges->push_back(new HomeKitDisplay(homekitAID));   
        _components.push_back(new KnxChannelDisplay(displayBridges, _channelIndex));
        break;
      }
      case 7:
      {
        Serial.println("Sensor");
        std::list<ISensorBridge *> *sensorBridges = new std::list<ISensorBridge *>();
        if (mode & Mode::Homekit)
          sensorBridges->push_back(new HomeKitSensor(homekitAID));   
        _components.push_back(new KnxChannelSensor(sensorBridges, _channelIndex));
        break;
      }
      default:
      {
        Serial.print("Error: Unkown device type ");
        Serial.println(deviceType);
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