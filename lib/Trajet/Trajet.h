#ifndef TRAJET_H
#define TRAJET_H

#include "Arduino.h"
#include "Mouvement.h"
#include "config.h"

/* Incrémenter l'orientation correspond à tourner de 90° vers la droite */
enum Orientation
{
    X_pos, // Orientation de base
    Y_pos,
    X_neg,
    Y_neg
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

class Trajet
{
    public:

        Trajet(Mouvement* _mouvement);
        ~Trajet();

        // void addLigne(SensDeplacement sens, double distance) ; // 
        void setOrientation(Orientation new_orientation); // rotation sur place
        

        void goToXvalue(double x, SensDeplacement sens);
        void goToYvalue(double y, SensDeplacement sens);
        void goToObjective(Objectif objectif);
        void returnToBase(); // go to (0,0) facing x_pos

        double getX();
        double getY();
        Orientation getOrientation();

        void printPosition();

        // execute la liste de mouvements

    private:
        PositionRobot position;
        Mouvement* mouvement;


        void deplacementBase(SensDeplacement sens); // avance d'un "step" dans une direction, update x,y       
};



#endif