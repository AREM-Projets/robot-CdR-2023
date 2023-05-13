#ifndef TRAJET_H
#define TRAJET_H

#include "vector"
#include "Arduino.h"

/* Incrémenter l'orientation correspond à tourner de 90° vers la droite */
    enum Orientation
    {
        X_pos, // Orientation de base
        Y_pos,
        X_neg,
        Y_neg
    };

    enum SensRotation
    {
        Gauche,
        Droite
    };

struct Objectif
{
    double x;
    double y;
    Orientation orientation;
};

struct PositionRobot
{
    double x;
    double y;
    Orientation orientation;
};
/*
class Odometrie
{
    public:
        Trajet();
        void addObjectif(Objectif objectif);
        bool estVide() const;


    private:
        PositionRobot position;
        std::vector<Objectif> objectifs;
  
};
*/

#endif
