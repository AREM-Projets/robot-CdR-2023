#include "Leds.h"

Leds::Leds(uint32_t pin_leds)
{
    pinMode(pin_leds, OUTPUT);
    pin = pin_leds;
}

void Leds::On()
{
    digitalWrite(pin, HIGH);
}

void Leds::Off()
{
    digitalWrite(pin, LOW);
}

void Leds::startTimer(uint32_t time_ms/*  = 90000 */)
{
    startTime_ms = millis();
    timerLength_ms = time_ms;
}

bool Leds::timerEndTest()
{
    if(millis() >= startTime_ms + timerLength_ms)
    {    
        On();
        return true;
    }
    return false;
}
