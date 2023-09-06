#include "HueDoorWindow.h"

HueDoorWindow::HueDoorWindow(HueBridge* hueBridge)
: hueBridge(hueBridge)
{
    
}

void HueDoorWindow::setup(uint8_t _channelIndex)
{
    espalexaDevice = new EspalexaDevice(_channel->getNameInUTF8(), [this](EspalexaDevice* d){update();}, EspalexaDeviceType::dimmable, 0);
    espalexaDevice->setState(false);
    hueBridge->espalexa.addDevice(_channel->channelIndex(), espalexaDevice);
 }

boolean HueDoorWindow::update()
{
    switch (espalexaDevice->getLastChangedProperty())
    {
        case EspalexaDeviceProperty::on:
            _channel->commandPosition(this, 100);
            break;
        case EspalexaDeviceProperty::off:
            _channel->commandPosition(this, 0);
            break;
         case EspalexaDeviceProperty::bri:
            _channel->commandPosition(this, espalexaDevice->getPercent());
            break;
    }
    return (true);
}

void HueDoorWindow::setMovement(DoorWindowMoveState movement)
{

}

void HueDoorWindow::setPosition(uint8_t position)
{
    espalexaDevice->setPercent(position);
    if (position == 0)
    {
        espalexaDevice->setState(false);
    }
    else
    {
        espalexaDevice->setState(true);
    }
}
