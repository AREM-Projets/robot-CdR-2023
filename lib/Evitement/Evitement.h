#ifndef EVITEMENT_H
#define EVITEMENT_H

#include "BlocMoteurs.h"
#include "Ultrasonic.h"
#include "Arduino.h"

void Evitement(Ultrasonic avant, Ultrasonic droite, Ultrasonic gauche, Ultrasonic arriere, BlocMoteurs* motors);

#endif