// Version récupérée sur le code des robots de la coupe 2022

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "config.h"

#ifndef TCA9548A_h
#include "TCA9548A.h"
#endif


typedef enum _direction
{
    FD,
    FG,
    BD,
    BG
}direction;

class MagneticEncodeurs
{
    public:
        MagneticEncodeurs(TCA9548A *I2CMux = NULL);
        void setZeroPosition();

        int64_t getDeltaTicks(direction dir, bool useMux = true);

        int64_t getSumTicks(direction dir);
        

        int64_t getAngle(direction dir);

        int64_t getLastDelta(direction dir){return _lastDelta[dir];};
        

        void test_tofs();

        void debugPrint();
    private:
        int64_t _processDeltaTicks(direction dir, bool useMux = true);
        
        int64_t _processDelta(uint16_t ticks, uint16_t lastTicks, direction dir);

        uint16_t _readAngle(direction dir, bool useMux = true);
        

        TCA9548A *_I2CMux;
       

        double _angle[4];
        

        double _angleZero[4];
        

        uint16_t _lastTicks[4];
        int64_t _lastDelta[4];
        int64_t _lastDerivate[4];
        uint16_t miss[4];
};


