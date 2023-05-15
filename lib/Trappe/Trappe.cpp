#include "Trappe.h"

#define POS_FERME 10
#define POS_OUVERTE 160


void ouvrir(Servo servo) {
    servo.write(POS_OUVERTE);

    /* Debug print */
    if(Serial)
    {
        Serial.println("Trappe ouverte");
    }
}
  
void fermer(Servo servo) {
    servo.write(POS_FERME);

    /* Debug print */
    if(Serial)
    {
        Serial.println("Trappe fermée");
    }
}