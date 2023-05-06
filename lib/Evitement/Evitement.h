#ifndef EVITEMENT_H
#define EVITEMENT_H

#define niveauEvi 30

#include "Arduino.h"

class Capteurs
{
    public:
        uint32_t Trig[4]; //Numéros de pins associés aux broches de trig des capteurs ultrasons             
        uint32_t Echo[4]; // Idem pour les broches d'Echo                          
        //Avant, Droite, Gauche, Arrière

        float RetourCapteur(int cote);  
        //Permet d'avoir la distance captée par un capteur en donnant le numéro du côté du robot

        float TousCapteurs();
        //Renseigne dans le tableau Distance les distance captées par tous les capteurs 

    private:
        float convertion(float valeur);
        //Convertie le temps fournit par les capteurs en une distance

};

#endif