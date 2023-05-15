#include "Leds.h"

Leds::Leds(uint32_t pin_leds)
{
    pinMode(pin_leds, OUTPUT);
    pin = pin_leds;

    /* Debug print */
    if(Serial)
    {
        Serial.println("Leds initialised");
    }
}

void Leds::On()
{
    digitalWrite(pin, HIGH);

    /* Debug print */
    if(Serial)
    {
        Serial.println("Led ON");
    }
}

void Leds::Off()
{
    digitalWrite(pin, LOW);

    /* Debug print */
    if(Serial)
    {
        Serial.println("Led OFF");
    }
}

void Leds::startTimer(uint32_t time_ms/*  = 90000 */)
{
    startTime_ms = millis();
    timerLength_ms = time_ms;

    /* Debug print */
    if(Serial)
    {
        Serial.printf("Started timer for Leds with time = %d ms ", time_ms);
        Serial.println();
    }
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
