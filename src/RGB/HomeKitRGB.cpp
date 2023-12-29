#include "HomeKitRGB.h"

struct rgb {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
};

struct hsv {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
};


hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;     
}

HomeKitRGB::HomeKitRGB(int device) :
    device(device)
{
}

void HomeKitRGB::setup(uint8_t _channelIndex)
{
    new SpanAccessory(device);
        new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(_channel->getNameInUTF8());
    new ServiceImplementation(this);
        power = new Characteristic::On();
        brighness = new Characteristic::Brightness();
        saturation = new Characteristic::Saturation();
        hue = new Characteristic::Hue();
}

boolean HomeKitRGB::update()
{
    if (brighness->updated() || saturation->updated() || hue->updated())
    {
        hsv hsv;
        hsv.h = brighness->getNewVal();
        hsv.s = ((double) saturation->getNewVal()) / 100.;
        hsv.v = ((double) hue->getNewVal()) / 100.;
        auto rgb = hsv2rgb(hsv);
        auto r = (uint32_t)(rgb.r * 255.);
        auto g = (uint32_t)(rgb.g * 255.);
        auto b = (uint32_t)(rgb.b * 255.);
        uint32_t rgbValue = (r << 16) | (g << 8) | (b);
        _channel->commandRGB(this, rgbValue);
    }
    else if (power->updated())
        _channel->commandPower(this, power->getNewVal());
    return true;
}

void HomeKitRGB::setPower(bool on)
{
    power->setVal(on);
}

void HomeKitRGB::setRGB(uint32_t rgbValue)
{
    if (rgbValue == 0)
    {
        power->setVal(true);
    }
    else
    {
        rgb rgb;
        rgb.r = ((rgbValue & 0xFF0000) >> 16) / 255.;
        rgb.g = ((rgbValue & 0x00FF00) >> 8) / 255.;
        rgb.b = ((rgbValue & 0x0000FF)) / 255.;
        auto hsv = rgb2hsv(rgb);
    
        saturation->setVal(hsv.s * 100.);
        brighness->setVal(hsv.v * 100.);
        hue->setVal(hsv.h);
        power->setVal(true);
    }
}

