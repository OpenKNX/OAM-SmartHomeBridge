#include "Bridge.h"
#include "KnxChannelBase.h"

KnxChannelBase::KnxChannelBase(uint16_t channelIndex)
    : Component((const char *)deviceName)
{
    _channelIndex = channelIndex;
    readKnxParameterString("DeviceName", ParamBRI_CHDeviceName, deviceName, sizeof(deviceName));
}

const std::string KnxChannelBase::name()
{
    return deviceName;
}