#ifndef ODOMETRIE_H
#define ODOMETRIE_H

/**
 * Version récupérée sur le code des robots de la coupe 2022
 * spécifique roues mecanum
 **/

#include <Arduino.h>
#include "config.h"
#include "EtatDynamiqueRobot.h"

#ifndef TCA9548A_h
#include "TCA9548A.h"
#endif

#include "MagneticEncodeurs.h"
#include "MPU6050.h"


#define TICKS_PAR_TOUR 4095 // correspond à la précision de l'encodeur magnétique
#define RAYON_ROUE 30 // Placeholder, a mesurer // en mm

#ifdef NOTRE_ROBOT
#define L1 102.5 // A MESURER // en mm ; Largeur entre le centre du robot et le milieu d'une roue (milieu vers gauche par rapport à l'avant)
#define L2 85 // A MESURER // en mm ; Longeur entre le centre du robot et le milieu d'une roue (milieu vers avant par rapport à l'avant)
#endif
#define HANDLERDELAY 10000

#define DISTANCE_PAR_TICK_ROUE (2.0*PI*(double)RAYON_ROUE/(double)TICKS_PAR_TOUR) // a mesurer // en mm/ticks (THEORIQUEMENT = 2*PI*RAYON_ROUE/TICKS_PAR_TOUR)

class Odometrie
{
    public:
        Odometrie(uint32_t min_update_period, MagneticEncodeurs* myencodeurs, bool lidar = true);

        ~Odometrie();
        bool update(bool useMPU = true);
        
        double getThetaRadian(bool useMPU = true) const;
        double getX() const;
        double getY() const;
        double getV() const;
        double getVX() const;
        double getVY() const;
        double getVTheta() const;
        double deltaAngleTo(double initial, double target) const;

        void debug();
        
        void setX(double x);
        void setY(double y);
        void setTheta(double theta);

        // Fonctions pour appliquer les calculs matriciels pour obtenir les vitesses du robot en fonction 
        // de celle des roues et vice versa
        void compute_encoders_to_robot(const double *w1, const double *w2, const double *w3, const double *w4, 
                                        double *vx, double *vy, double *wz);
        void compute_robot_to_encoders(const double *vx, const double *vy, const double *wz, 
                                        double *w1, double *w2, double *w3, double *w4);

        void setRayonRoues(double newRayon);
        void setL1pL2(double newL1pL2);
        double getRayonRoues();
        double getL1pl2();
        uint32_t getMin_update_period();

        void compute_robot_to_encoders_AbsoluteRef(const double *vx, const double *vy, const double *wz, 
                                        double *w1, double *w2, double *w3, double *w4);

        void handler();

        bool MPUlaunched() { return mpuLaunchedOk; };
        bool MPUStable() { return mpuStable; };

        double getThetaEnco(){return thetaEnco;};
        double getThetaMPU(){return thetaMPU;};

     

    private:


        bool useLidar;
        MagneticEncodeurs *encodeurs;

        EtatDynamiqueRobot etat;

        uint32_t _min_update_period;
        uint32_t _last_update_etat; // il s'agit de la date de la dernière mise à jour de position/rotation/vitesse

        double RayonRoues;
        double L1pL2;

        MPU6050 accelgyro;
        uint8_t devStatus;
        uint8_t mpuIntStatus;
        uint16_t packetSize;   
        Quaternion q;           // [w, x, y, z]         quaternion container
        float euler[3];
        uint16_t fifoCount;
        uint8_t fifoBuffer[64]; 
        char msgRasp[50];
        
        uint32_t _last_update_MPU;

        double lasteuler;
        bool mpuStable;
        bool mpuLaunchedOk;
        bool mpuCalibDone;
        int mpuCountOk;

        double offsetEuler;

        double thetaEnco;
        double thetaMPU;

        

};





#endif