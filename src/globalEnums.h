#pragma once

enum OutputSendMode
{
    OnAndOff,
    OnlyOn,
    OnlyOff,
    InvertedOnAndOff,
    InvertedOnlyOnAfterInversion,
    InvertedOnlyOffAfterInversion,
};

enum DeviceType
{
    Switch,
    Dimmer
};