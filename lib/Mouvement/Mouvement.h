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

        void forward(double distance/*unit?*/);
        void rotate(SensRotation sens);

        void faceDirection(Orientation orientation);

    private:

        BlocMoteurs* motors;
        PositionRobot position;

        double temp_measure; // for simple methods like forward...

        void step_forward();

};

#endif