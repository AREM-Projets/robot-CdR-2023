#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#include "Ultrasonic.h"
#include "config.h"
#include "BlocMoteurs.h"
#include "MagneticEncodeurs.h"
#include "Odometrie.h"
#include "ControleurPID.h"
#include "Evitement.h"


Ultrasonic ultrasonic(D7, D8);
int distance;

void setup() {
    pinMode(D7, OUTPUT); //Trig
    pinMode(D8, INPUT); //Echo
    Serial.begin(9600);

}

void loop() {
    
    distance = ultrasonic.read();

    Serial.print("Distance in CM: ");
    Serial.println(distance);

    
}
