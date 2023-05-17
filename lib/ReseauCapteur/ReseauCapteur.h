#ifndef EVITEMENT_H
#define EVITEMENT_H

#include "BlocMoteurs.h"
#include "Ultrasonic.h"
#include "Arduino.h"

class ReseauCapteur
{
public:
    ReseauCapteur(Ultrasonic Avant1, Ultrasonic Avant2, Ultrasonic Arriere1, Ultrasonic Arriere2, Ultrasonic Gauche, Ultrasonic Droite);

    /* On fonce pas dans un obstacle, on s'arrête. Renvoie le temps d'arrêt. */
    uint32_t EvitementTranslation(int signe, BlocMoteurs* motors);

    /* Si on a un robot trop proche à droite ou gauche on ne tourne pas et on attend */
    void EvitementRotation();

    bool testCotes();

    Ultrasonic Avant1;
    Ultrasonic Avant2;
    Ultrasonic Arriere1;
    Ultrasonic Arriere2;
    Ultrasonic Gauche;
    Ultrasonic Droite;
};

#endif
