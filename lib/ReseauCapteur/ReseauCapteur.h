#ifndef EVITEMENT_H
#define EVITEMENT_H

#include "BlocMoteurs.h"
#include "Ultrasonic.h"
#include "Arduino.h"

class ReseauCapteur
{
public:
    ReseauCapteur(Ultrasonic Avant1, Ultrasonic Avant2, Ultrasonic Arriere1, Ultrasonic Arriere2, Ultrasonic Gauche, Ultrasonic Droite);
    void EvitementTranslation(int signe, BlocMoteurs* motors);
    void EvitementRotation(int signe, BlocMoteurs* motors);

    Ultrasonic Avant1;
    Ultrasonic Avant2;
    Ultrasonic Arriere1;
    Ultrasonic Arriere2;
    Ultrasonic Gauche;
    Ultrasonic Droite;
};

#endif
