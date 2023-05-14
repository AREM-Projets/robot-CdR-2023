#ifndef TRAJET_H
#define TRAJET_H

#include "Arduino.h"
#include "Mouvement.h"

/* Erreur max sur un déplacement, doit être >= QUANTUM_DIST */
#define ERREUR_MAX QUANTUM_DIST

/* Délai automatique après une action, en ms */
#define AUTOMATIC_DELAY 100

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
        void faceDirection(Orientation new_orientation); // rotation sur place
        

        void goToXvalue(double x);
        void goToYvalue(double y);
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