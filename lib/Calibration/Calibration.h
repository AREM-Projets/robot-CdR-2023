#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "BlocMoteurs.h"
#include "Mouvement.h"
#include "Arduino.h"

#define QUANTUM_TEMPS   100      // pas temporel des roues
#define QUANTUM_DIST    100      // pas spatial des roues
#define DUREE_ROTATION  1000
#define VITESSE 0.2       //eventuellement plus si c'est trop lent (entre 0 et 1)      // vitesse test 

class Calibration
{
    public:
        Calibration(BlocMoteurs* motors);

        void test_rotations();
        void test_deplacements();
        void test_carre();
};

#endif