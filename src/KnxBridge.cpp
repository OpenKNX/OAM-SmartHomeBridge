
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
#include "Helper.h"

#include "Logic.h"

const uint8_t cFirmwareMajor = 1;    // 0-31
const uint8_t cFirmwareMinor = 0;    // 0-31
const uint8_t cFirmwareRevision = 0; // 0-63

Logic gLogic;

uint32_t heartbeatDelay = 0;
uint32_t startupDelay = 0;

// true solange der Start des gesamten Moduls verzögert werden soll
bool startupDelayfunc()
{
  return !delayCheck(startupDelay, getDelayPattern(LOG_StartupDelayBase));
}

void ProcessHeartbeat()
{
  // the first heartbeat is send directly after startup delay of the device
  if (heartbeatDelay == 0 || delayCheck(heartbeatDelay, getDelayPattern(LOG_HeartbeatDelayBase)))
  {
    // we waited enough, let's send a heartbeat signal
    knx.getGroupObject(LOG_KoHeartbeat).value(true, getDPT(VAL_DPT_1));

    heartbeatDelay = millis();
  }
}

void ProcessReadRequests()
{
  // this method is called after startup delay and executes read requests, which should just happen once after startup
  static bool sCalledProcessReadRequests = false;
  if (!sCalledProcessReadRequests)
  {
    // we go through all IO devices defined as outputs and check for initial read requests
    sCalledProcessReadRequests = true;
  }
  gLogic.processReadRequests();
}

bool processDiagnoseCommand()
{
  char *lBuffer = gLogic.getDiagnoseBuffer();
  bool lOutput = false;
  if (lBuffer[0] == 'v')
  {
    // Command v: return filmware version, do not forward this to logic,
    // because it means firmware version of the outermost module
    sprintf(lBuffer, "VER [%d] %d.%d", cFirmwareMajor, cFirmwareMinor, cFirmwareRevision);
    lOutput = true;
  }
  else
  {
    // let's check other modules for this command
    lOutput = gLogic.processDiagnoseCommand();
  }
  return lOutput;
}

void ProcessDiagnoseCommand(GroupObject &iKo)
{
  // this method is called as soon as iKo is changed
  // an external change is expected
  // because this iKo also is changed within this method,
  // the method is called again. This might result in
  // an endless loop. This is prevented by the isCalled pattern.
  static bool sIsCalled = false;
  if (!sIsCalled)
  {
    sIsCalled = true;
    //diagnose is interactive and reacts on commands
    gLogic.initDiagnose(iKo);
    if (processDiagnoseCommand())
      gLogic.outputDiagnose(iKo);
    sIsCalled = false;
  }
};

void processKoCallback(GroupObject &iKo)
{
  Component::receiveAll(iKo);
  gLogic.processInputKo(iKo);
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

void appLoop()
{
#ifdef KNXenable
  if (!knx.configured())
    return;

  // handle KNX stuff
  if (startupDelayfunc())
    return;
#endif
  // at this point startup-delay is done
  // we process heartbeat
  ProcessHeartbeat();
  ProcessReadRequests();
  gLogic.loop();
}
