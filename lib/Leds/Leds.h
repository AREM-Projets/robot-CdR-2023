#ifndef LEDS_H
#define LEDS_H

#include "Arduino.h"

class Leds
{
    public:
        Leds(uint32_t pin_leds);
        void On();
        void Off();
        //void Off();

        /* Lance un timer de 90s. Le temps peut être modifié pour des tests */
        void startTimer(uint32_t time_ms/*  = 90000 */);

        /* Allume les Leds si le timer est terminé, renvoie "timer terminé"(bool) */
        bool timerEndTest();

    private:

        uint32_t pin;

        uint32_t startTime_ms;
        uint32_t timerLength_ms;
};

#endif