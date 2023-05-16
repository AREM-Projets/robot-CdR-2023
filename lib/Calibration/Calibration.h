#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "BlocMoteurs.h"
#include "Mouvement.h"
#include "Arduino.h"
#include "config.h"

class Calibration
{
    public:
        Calibration(BlocMoteurs* motors);
        ~Calibration();

        void test_rotations();
        void test_deplacements();
        void test_carre();

    private:
        BlocMoteurs* moteurs;
        Mouvement* mouvement;
};

#endif