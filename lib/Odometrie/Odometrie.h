#ifndef TRAJET_H
#define TRAJET_H

#include "vector"
#include "Arduino.h"
#include "Mouvement.h"

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

class Odometrie
{
    public:
        Odometrie(Mouvement* _mouvement);

        void addObjectif(Objectif objectif);
        bool estVide() const;


    private:

        void faceDirection(Orientation orientation);

        Mouvement* mouvement;

        PositionRobot position;
        std::vector<Objectif> objectifs;
  
};


#endif
