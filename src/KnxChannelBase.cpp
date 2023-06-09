#include "Bridge.h"
#include "KnxChannelBase.h"

KnxChannelBase::KnxChannelBase(uint16_t goOffset, uint32_t parameterOffet)
    : Component((const char *)deviceName, goOffset)
{
    readKnxParameterString("DeviceName", parameterOffet + BRI_CHDeviceName, deviceName, sizeof(deviceName));
}