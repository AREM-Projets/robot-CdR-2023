#include "Trappe.h"

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