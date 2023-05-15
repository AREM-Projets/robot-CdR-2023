#ifndef MOUVEMENT_H
#define MOUVEMENT_H
// Constantes à modifier selon le test robot

#include "BlocMoteurs.h"
#include "config.h"

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

        void deplacement(SensDeplacement sens, double distance/*unit?*/);
        void rotate(SensRotation sens);

        

    private:

        BlocMoteurs* motors;
        PositionRobot position;

        double temp_measure; // for simple methods like forward...

        

};

#endif