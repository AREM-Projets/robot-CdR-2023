/**
 * Version récupérée sur le code des robots de la coupe 2022
 * spécifique aux roues mecanum
 **/

#include "EtatDynamiqueRobot.h"



double EtatDynamiqueRobot::getX() const
{
    return x_mm;
}

double EtatDynamiqueRobot::getY() const
{
    return y_mm;
}
