#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#include "config.h"
#include "BlocMoteurs.h"
#include "Mouvement.h"
SPIClass* dev_spi;
BlocMoteurs* motors;
Mouvement* mouvement;

void setup()
{
    dev_spi = new SPIClass(D11, D12, D13);
    dev_spi->begin();
    motors = new BlocMoteurs(dev_spi);
    mouvement = new Mouvement(motors);

    mouvement->forward(100);
    delay(1000);
    mouvement->rotate(Droite);
    delay(1000);
    mouvement->forward(100);
}

void loop()
{

}