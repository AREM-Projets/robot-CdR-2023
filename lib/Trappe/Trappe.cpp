#include "Trappe.h"

<<<<<<< HEAD
#define POS_FERME 10
#define POS_OUVERTE 160

//J'ai besoin de décrire cette bibliothèque ?

=======
>>>>>>> 678a0c455cbdc3c076445d7874dd5afab277a222
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