#ifndef ODOMETRIE_H
#define ODOMETRIE_H


#include <vector>
#include "Arduino.h"
#include "Mouvement.h"
#include "Trajet.h"

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

        // execute la liste de mouvements
        void startSequence();

        bool estVide() const;


    private:

        Mouvement* mouvement;
        PositionRobot position;

        std::vector<Objectif> objectifs;

        void faceDirection(Orientation orientation);

};


#endif