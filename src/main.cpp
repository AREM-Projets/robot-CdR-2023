#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#include "Ultrasonic.h"
#include "config.h"
#include "BlocMoteurs.h"
#include "Mouvement.h"
#include "ReseauCapteur.h"
#include "Trappe.h"

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

int state;

void setup()
{
    state = false;

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
    motors->motors_stop_hard_hiz();
 
    /* Init capteurs */
    /* capteur_front_left = new Ultrasonic(pinUltrasonFLT, pinUltrasonFLE);
    capteur_front_right = new Ultrasonic(pinUltrasonFRT, pinUltrasonFRE);

    capteurs = new ReseauCapteur(capteur_front_left, capteur_front_right); */

    /* Init mouvement */
    mouvement = new Mouvement(motors, capteurs);

    /* Init serial */
    Serial.begin(115200);
    //while (!Serial);
    

    /* Init servo */
    /* myservo.attach(pinServoPanier);
    delay(500);
    fermer(myservo); */

    /* On attend le signal de start */
    Serial.println("Robot initialised");
}


void loop()
{

    motors->motors_stop_hard_hiz(); // Déplacement libre avant le début 
    while(digitalRead(pinStarter) == HIGH) 
    {
        Serial.println("Waiting for start");
        // Serial.println(digitalRead(pinStarter));
        delay(200);
    }

    timer_match = millis();
    Serial.println("Match started !");

    /*--- Code de la stratégie ---*/

    // // Servo
    // delay(1000);
    // ouvrir(myservo);
    // delay(3000);
    // fermer(myservo);

    delay(1000);
    mouvement->deplacement(Droite, 2000);
    switch (state)
    {
    case false:
        mouvement->deplacement(Avancer, 6300);
        state = true;
        break;
    
    case true:
        mouvement->deplacement(Reculer, 6300);
        state = false;
        break;
    }
    mouvement->deplacement(Gauche, 2000);


    // Version CP 
    /* switch (state)
    {
    case 0:
        mouvement->deplacement(Droite, 12000);
        state++;
        break;
    
    case 1:
        mouvement->deplacement(Avancer, 6300);
        state++;
        break;

    case 2:
        mouvement->deplacement(Gauche, 12000);
        state++;
        break;

    case 3:
        mouvement->deplacement(Reculer, 6300);
        state = 0;
        break;

    default:
        state = 0;
        break;
    }    */


    /*--- Fin de la stratégie... ---*/
    Serial.println("Done");
    while (digitalRead(pinStarter) == 0)
    {
        Serial.println("Waiting for next match");
        delay(200);
    }   
}