#include "HueJalousie.h"

HueJalousie::HueJalousie(HueBridge* bridge) :
    HueRolladen(bridge)
{
}

void HueJalousie::initialize(KnxChannelRolladen *rolladenDevice)
{
    // Do nothing, the function below will be called to initialize the base.
}

void HueJalousie::initialize(KnxChannelJalousie *jalousieDevice)
{
    HueRolladen::initialize(jalousieDevice);
}

boolean HueJalousie::update()
{
    return HueRolladen::update();
}

void HueJalousie::setPosition(uint8_t position)
{
    HueRolladen::setPosition(position);
}

void HueJalousie::setMovement(MoveState movement)
{
    HueRolladen::setMovement(movement);
}

void HueJalousie::setSlatPosition(uint8_t position)
{

}

