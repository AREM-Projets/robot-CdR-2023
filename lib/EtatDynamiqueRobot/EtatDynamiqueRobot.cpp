/**
 * Version récupérée sur le code des robots de la coupe 2022
 * spécifique aux roues mecanum
 **/

#include "EtatDynamiqueRobot.h"

EtatDynamiqueRobot::EtatDynamiqueRobot(double x, double y, double theta) : x(x), y(y), theta(theta)
{
    // On s'assure que les angles soient bien dans ]-PI, PI]
    while(theta <= - PI) { theta += 2.0 * PI; }
    while(theta > PI) { theta -= 2.0 * PI; }

    v_xy = 0;
    v_theta = 0;
    v_x = 0;
    v_y=0;

    last_dt = micros();
}

void EtatDynamiqueRobot::versPosition(double n_x, double n_y, double n_theta, uint32_t dt)
{
    // On s'assure que les angles soient bien dans ]-PI, PI]
    while(n_theta <= - PI) { n_theta += 2.0 * PI; }
    while(n_theta > PI) { n_theta -= 2.0 * PI; }

    v_xy = sqrt( pow(n_x - x, 2) + pow(n_y - y, 2)) / dt;
    v_x = (n_x - x)/dt; 
    v_y = (n_y - y)/dt;
    v_theta = (n_theta - theta) / dt;

    x = n_x;
    y = n_y;
    theta = n_theta;

    last_dt = dt;
}

void EtatDynamiqueRobot::setPosition(double n_x, double n_y, double n_theta)
{
    // On s'assure que les angles soient bien dans ]-PI, PI]
    while(n_theta <= - PI) { n_theta += 2.0 * PI; }
    while(n_theta > PI) { n_theta -= 2.0 * PI; }

    x = n_x;
    y = n_y;
    theta = n_theta;

    v_xy = 0;
    v_theta = 0;
}

void EtatDynamiqueRobot::translatePosition(double n_x, double n_y, double n_theta)
{
    // On s'assure que les angles soient bien dans ]-PI, PI]
    while(n_theta <= - PI) { n_theta += 2.0 * PI; }
    while(n_theta > PI) { n_theta -= 2.0 * PI; }

    x = n_x;
    y = n_y;
    theta = n_theta;
}

/**
 * @brief Calcul l'angle le plus petit pour atteindre la cible depuis l'angle initial. Le résultat est garanti dans ]-PI, PI].
 */

double EtatDynamiqueRobot::deltaAngleTo(double initial, double target) const
{
    // On s'assure que les angles soient bien dans ]-PI, PI]
    while(initial <= - PI) { initial += 2.0 * PI; }
    while(initial > PI) { initial -= 2.0 * PI; }

    while(target <= - PI) { target += 2.0 * PI; }
    while(target > PI) { target -= 2.0 * PI; }

    // Chemin naïf vers l'objectif
    double delta = (target - initial);
    
    // Dans le cas ou il serait plus simple de partir dans l'autre sens
    if(delta <= -PI)
        delta += 2.0 * PI;
    if(delta > PI)
        delta -= 2.0 * PI;

    return delta;
    
}

double EtatDynamiqueRobot::getThetaRadian() const
{
    return theta;
}

double EtatDynamiqueRobot::getX() const
{
    return x;
}

double EtatDynamiqueRobot::getY() const
{
    return y;
}

double EtatDynamiqueRobot::getV() const
{
    return v_xy;
}

double EtatDynamiqueRobot::getVTheta() const
{
    return v_theta;
}

double EtatDynamiqueRobot::getVX() const
{
    return v_x;

}
double EtatDynamiqueRobot::getVY() const
{
    return v_y;
}
