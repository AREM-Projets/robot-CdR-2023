#ifndef MOUVEMENT_H
#define MOUVEMENT_H
// Constantes à modifier selon le test robot

#include "BlocMoteurs.h"

#define QUANTUM_TEMPS   100      // pas temporel des roues
#define QUANTUM_DIST    100      // pas spatial des roues
#define DUREE_ROTATION  1000    // explicite
#define VITESSE 0.3 // Vitesse des déplacements (avancer/reculer), entre 0 et 1.

// Enumération des variables d'état

enum SensDeplacement
{
    Reculer,
    Avancer
};

enum SensRotation
{
    Gauche,
    Droite
};

// Classe mouvement pour des déplacements de base
class Mouvement
{
    public:
        Mouvement(BlocMoteurs* _moteurs);
        ~Mouvement();

        void step_forward(); // step : Avancer de QUANTUM_DISTANCE

        void deplacement(SensDeplacement sens, double distance/*unit?*/);
        void rotate(SensRotation sens);

        // signe = 1 pour avancer et -1 pour reculer
        void mouvementElementaire(int signe);

    private:

        BlocMoteurs* motors;

        double temp_measure; // for simple methods like forward...

        

};

#endif