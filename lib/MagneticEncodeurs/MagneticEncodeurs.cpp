// Version récupérée sur le code des robots de la coupe 2022

#include "MagneticEncodeurs.h"

MagneticEncodeurs::MagneticEncodeurs(TCA9548A *I2CMux)
{
    if(I2CMux == NULL)
    {
        _I2CMux = new TCA9548A();   
        _I2CMux->begin(Wire);
        _I2CMux->closeAll();
    }
    else
    {
        _I2CMux = I2CMux;
        _I2CMux->closeAll();
    }



    for(int i = 0; i<4; i++)
    {
        _angle[i]=0;
        _angleZero[i]=0;
        _lastDelta[i]=0;
        _lastDerivate[i]=0;
        _lastTicks[i]= _readAngle((direction)i,true);
        miss[i] = 0;
    }

}

void MagneticEncodeurs::setZeroPosition()
{
    for(int i = 0; i<4; i++)
    {
        _angleZero[i] = _angle[i];
    }
    
}

int64_t MagneticEncodeurs::_processDeltaTicks(direction dir, bool useMux)
{
    uint16_t ticks = _readAngle(dir, useMux);


    int64_t delta = 0;
    if(dir == FG || dir == BG)
    {
        delta = - _processDelta(ticks, _lastTicks[dir],dir);
    }
    else
    {
        delta = _processDelta(ticks, _lastTicks[dir],dir);
    }

    _lastTicks[dir] = ticks;
    _angle[dir] = ticks * 2 * PI / 4095.0;

    return delta;
}



int64_t MagneticEncodeurs::_processDelta(uint16_t ticks, uint16_t lastTicks, direction dir)
{
    int64_t delta = 0;
    int overflow = 0;
    
    // On détecte du mieux que l'on peut les problèmes d'overflow du compteur CNT
    if(ticks < 0x0FF && lastTicks > 0x8FF) // CNT a surement overflow à droite
    {
        delta = (int64_t) ( (0x0FFF - lastTicks) + ticks + 1);
        overflow = 1;
    }
    else if(ticks > 0x8FF && lastTicks < 0x0FF) // CNT a surement overflow à gauche
    {
        delta = - (int64_t) ( (0x0FFF - ticks) + lastTicks + 1);
        overflow = 1;
    }
    else
    {
        delta = ((int64_t) ticks) - ((int64_t) lastTicks);
    }

    if( overflow != 1 && abs(delta) > 150 + abs(_lastDelta[dir])*0.2 && miss[dir] < 2 )
    {
        delta = _lastDelta[dir];
        miss[dir]+=1;
    }
    else
    {
        if( abs(delta-_lastDelta[dir]) > 150 && miss[dir] < 2)
        {
            delta = _lastDelta[dir];
            miss[dir]+=1;
        }
        else
        {
            miss[dir] = 0;
            _lastDelta[dir] = delta;

        }


    }


    return delta;
}

int64_t MagneticEncodeurs::getDeltaTicks(direction dir, bool useMux)
{
    return _processDeltaTicks(dir, useMux);
   
}

int64_t MagneticEncodeurs::getAngle(direction dir)
{
    _processDeltaTicks(dir);
    return _angle[dir] - _angleZero[dir];
}


void MagneticEncodeurs::debugPrint()
{
    char* dirToTxt2[4] = {"FD", "FG", "BD", "BG"};
    int sumtime = 0;
    safePrintSerial("$");
    for(int i = 0; i<4; i++)
    {
        safePrintSerial(" ");
        safePrintSerial(_readAngle((direction)i,true)); 

    }
    safePrintSerialln(";");


}

uint16_t MagneticEncodeurs::_readAngle(direction dir, bool useMux)
{
    uint16_t angle = 0;

    // Avec le circuit électrique du multiplexeur I2C utilisé : channel 0
    if(useMux)
    {
        _I2CMux->closeAll();
        _I2CMux->openChannel(dir);
    }

    // Pour placer le curseur sur le registre des 4 bits hauts de l'angle raw
    Wire.beginTransmission(byte(0x36)); //Adresse de l'encodeur magnetique AS5600 (ecriture) + Start Condition
    Wire.write(byte(0x0C)); // Acces au registre 8
    Wire.endTransmission(); // end condition

    Wire.requestFrom(byte(0x36),1);
    while(Wire.available()==0);
    angle = Wire.read();

    // Pour placer le curseur sur le registre des 4 bits hauts de l'angle raw
    Wire.beginTransmission(byte(0x36)); //Adresse de l'encodeur magnetique AS5600 (ecriture) + Start Condition
    Wire.write(byte(0x0D)); // Acces au registre 8
    Wire.endTransmission(); // end condition

    Wire.requestFrom(byte(0x36),1);
    while(Wire.available()==0);
    angle = (angle << 8) | Wire.read();

    return angle;
}

