#ifndef EVITEMENT_H
#define EVITEMENT_H

#include "BlocMoteurs.h"
#include "Ultrasonic.h"
#include "Arduino.h"
#include "config.h"

void Evitement(Ultrasonic* capteurs, BlocMoteurs* motors);

#endif