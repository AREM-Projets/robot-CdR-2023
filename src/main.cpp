#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#include "config.h"
#include "BlocMoteurs.h"
#include "MagneticEncodeurs.h"
#include "Odometrie.h"
#include "ControleurPID.h"
#include "Evitement.h"

void setup() {
    pinMode(D7, OUTPUT); //Trig
    pinMode(D8, INPUT); //Echo
    Serial.begin (115200);
}

void loop() {
    Capteurs Reseau;
    Reseau.Trig[0] = D7;
    Reseau.Echo[0] = D8;

    Serial.println(Reseau.RetourCapteur(0));
}