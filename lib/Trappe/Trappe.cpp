#include "Trappe.h"

#define POS_FERME 10
#define POS_OUVERTE 160


void ouvrir(Servo servo) {
    servo.write(POS_OUVERTE);
}
  
void fermer(Servo servo) {
    servo.write(POS_FERME);
}