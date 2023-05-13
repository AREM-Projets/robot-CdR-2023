#ifndef ETATDYNAMIQUEROBOT
#define ETATDYNAMIQUEROBOT

/**
 * Version récupérée sur le code des robots de la coupe 2022
 * spécifique aux roues mecanum
 **/

#include <Arduino.h>
#include "BlocMoteurs.h"

#define MEAN_SAMPLES 35

/* Tourner à droite : ++ et à gauche : -- (toujours % 4)*/
enum Orientation 
{
    X_pos,
    Y_pos,
    X_neg,
    Y_neg
};


class EtatDynamiqueRobot
{
    public:

        EtatDynamiqueRobot(BlocMoteurs moteurs);

        
       
        double getOrientation() const;
        double getX() const;
        double getY() const;

    private:
        double x_mm;
        double y_mm;
        Orientation orientation;

        // uint32_t last_dt;
};

#endif