#include "HueRGB.h"

HueRGB::HueRGB(HueBridge* hueBridge)
: hueBridge(hueBridge)
{
    
}

void HueRGB::setup(uint8_t _channelIndex)
{
    espalexaDevice = new EspalexaDevice(_channel->getNameInUTF8(), [this](EspalexaDevice* d){update();}, EspalexaDeviceType::color, 0);
    espalexaDevice->setState(false);
    espalexaDevice->setColor((uint8_t) 0xFF, (uint8_t) 0xFF, (uint8_t) 0xFF);
    hueBridge->espalexa.addDevice(_channel->channelIndex(), espalexaDevice);
}

boolean HueRGB::update()
{
    switch (espalexaDevice->getLastChangedProperty())
    {
        case EspalexaDeviceProperty::on:
            _channel->commandPower(this, true);
            break;
        case EspalexaDeviceProperty::off:
            _channel->commandPower(this, false);
            break;
        default:
            _channel->commandRGB(this, espalexaDevice->getRGB());
            break;
    }
    return (true);
}

void HueRGB::setRGB(uint32_t rgb)
{
    uint8_t r = (rgb & 0xFF0000) >> 16;
    uint8_t g = (rgb & 0x00FF00) >> 8;
    uint8_t b = rgb & 0x0000FF;
    Serial.println(b);
    espalexaDevice->setColor(r, g, b);
    if (rgb == 0)
    {
        espalexaDevice->setState(false);
    }
    else
    {
        espalexaDevice->setState(true);
    }
}

void HueRGB::setPower(bool power)
{
    espalexaDevice->setState(power);
}