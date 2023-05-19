#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#include "Ultrasonic.h"
#include "config.h"
#include "BlocMoteurs.h"
#include "Mouvement.h"
#include "ReseauCapteur.h"
#include "Trappe.h"
#include "ActionneurAvant.h"
#include "Leds.h"

/* Moteurs */
SPIClass* dev_spi;
BlocMoteurs* motors;

/* Capteurs ultrasons */
Ultrasonic* capteur_front_left;
Ultrasonic* capteur_front_right;
ReseauCapteur* capteurs;

/* Mouvement */
Mouvement* mouvement;

/* Servo */
Servo myservo;

uint32_t timer_match = 0;

void setup()
{

    /* Configure pins */ 
    pinMode(pinUltrasonFLE, INPUT);
    pinMode(pinUltrasonFLT, OUTPUT);

    pinMode(pinServoPanier, OUTPUT);

    pinMode(pinUltrasonFRE, INPUT);
    pinMode(pinUltrasonFRT, OUTPUT);

    pinMode(pinStarter, INPUT_PULLUP);


    /* Init moteurs */
    dev_spi = new SPIClass(D11, D12, D13);
    dev_spi->begin();
    motors = new BlocMoteurs(dev_spi);
 
    /* Init capteurs */
    capteur_front_left = new Ultrasonic(pinUltrasonFLT, pinUltrasonFLE);
    capteur_front_right = new Ultrasonic(pinUltrasonFRT, pinUltrasonFRE);

    capteurs = new ReseauCapteur(capteur_front_left, capteur_front_right);

    /* Init mouvement */
    mouvement = new Mouvement(motors, capteurs);

    /* Init serial */
    Serial.begin(115200);
    //while (!Serial);
    

    /* Init servo */
    myservo.attach(pinServoPanier);
    delay(500);
    fermer(myservo);

    /* On attend le signal de start */
    Serial.println("Robot initialised");

    while(digitalRead(pinStarter) == HIGH) 
    {
        Serial.print("pinStarter : ");
        Serial.println(digitalRead(pinStarter));
        delay(100);
    }

    timer_match = millis();
    Serial.println("Match started !");

    /* Code de la stratégie */

    // Servo
    delay(1000);
    ouvrir(myservo);
    delay(3000);
    fermer(myservo);

    // Demi tour etc.
    delay(1000);
    mouvement->deplacement(Avancer, 2000);
    mouvement->rotate(Droite);
    mouvement->rotate(Droite);
    mouvement->deplacement(Avancer, 1950);

    Serial.println("Done");
}


void loop()
{
       
}