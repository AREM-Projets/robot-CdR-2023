#ifndef MOUVEMENT_H
#define MOUVEMENT_H
// Constantes à modifier selon le test robot

#include "BlocMoteurs.h"

#define QUANTUM_TEMPS   100      // pas temporel des roues
#define QUANTUM_DIST    100      // pas spatial des roues
#define DUREE_ROTATION  1000    // explicite
#define VITESSE 0.3 // Vitesse des déplacements (avancer/reculer), entre 0 et 1.

// Enumération des variables d'état

enum Orientation
{
    X_pos,
    Y_pos,
    X_neg,
    Y_neg
};

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

struct PositionRobot
{
    double x;
    double y;
    Orientation orientation;
};

class Mouvement
{
    public:
        Mouvement(BlocMoteurs* _moteurs);
        ~Mouvement();

        void step_forward(); // step : Avancer de QUANTUM_DISTANCE

        void Mouvement::deplacement(SensDeplacement sens, double distance/*unit?*/);
        void rotate(SensRotation sens);

        

    private:

        BlocMoteurs* motors;
        PositionRobot position;

        double temp_measure; // for simple methods like forward...

        

};

#endif