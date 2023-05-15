#include "Trappe.h"

void ouvrir(Servo servo) {
    servo.write(POS_OUVERTE);
}
  
void fermer(Servo servo) {
    servo.write(POS_FERME);
}