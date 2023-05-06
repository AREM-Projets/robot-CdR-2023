#include "Evitement.h"

float Capteurs::convertion(float valeur)
{
    return(valeur/(2*340));
}

float Capteurs::RetourCapteur(int cote)
{
    float DistanceCapt;
    
    // Émission d'un signal de durée 10 microsecondes
    digitalWrite(Trig[cote], LOW);
    delayMicroseconds(5);
    digitalWrite(Trig[cote], HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig[cote], LOW);

    // Écoute de l'écho
    DistanceCapt = convertion(pulseIn(Echo[cote], HIGH));

    return DistanceCapt;
}
