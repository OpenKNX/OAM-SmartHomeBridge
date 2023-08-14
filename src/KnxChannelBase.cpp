#include "Bridge.h"
#include "KnxChannelBase.h"

KnxChannelBase::KnxChannelBase(uint16_t channelIndex)
    : Component((const char *)deviceName)
{
    _channelIndex = channelIndex;
    readKnxParameterString("DeviceName", ParamBRI_CHDeviceName, deviceName, sizeof(this->deviceName));
    Serial.println(deviceName);
    Serial.println(strlen((const char *) ParamBRI_CHDeviceName));
}

const std::string KnxChannelBase::name()
{
    return deviceName;
}