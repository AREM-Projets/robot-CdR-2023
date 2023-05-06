#ifndef ETATDYNAMIQUEROBOT
#define ETATDYNAMIQUEROBOT

/**
 * Version récupérée sur le code des robots de la coupe 2022
 * spécifique aux roues mecanum
 **/

#include <Arduino.h>

#define MEAN_SAMPLES 35

class EtatDynamiqueRobot
{
    public:
        EtatDynamiqueRobot(double x, double y, double theta);

        // Pour passer vers un nouvel etat
        // Effectue le calcul des nouvelles vitesses
        void versPosition(double n_x, double n_y, double n_theta, uint32_t dt);
        void setPosition(double n_x, double n_y, double n_theta);
        void translatePosition(double n_x, double n_y, double n_theta);

        double getThetaRadian() const;
        double getX() const;
        double getY() const;
        double getV() const;
        double getVTheta() const;
        double getVX() const;
        double getVY() const;

        double deltaAngleTo(double initial, double target) const;

    private:
        double x;
        double y;
        double theta;
        
        double v_xy;
        double v_theta;

        double v_x;
        double v_y;

        uint32_t last_dt;
};

#endif