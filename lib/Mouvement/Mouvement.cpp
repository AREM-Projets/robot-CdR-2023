#include "Mouvement.h"


Mouvement::Mouvement(BlocMoteurs* moteurs)
{
    // Set starting position to (0,0) facing positive x
    position.x = 0;
    position.y = 0;
    position.orientation = X_pos;

    // set motors to the specified motor
    motors = moteurs;
}

Mouvement::~Mouvement()
{
    delete motors;
}

void Mouvement::step_forward()
{
    motors->motors_on();
    motors->commande_vitesses(VITESSE, VITESSE, VITESSE, VITESSE);
    delay(QUANTUM_TEMPS);
    motors->motors_stop_low_hiz();
    // sous réserve de définition (ajouter orientation dans la même classe) :
    switch (position.orientation)
    {
        case X_pos:
            position.x += QUANTUM_DIST;
            break;

        case Y_pos:
            position.y += QUANTUM_DIST;
            break;

        case X_neg:
            position.x -= QUANTUM_DIST;
            break;

        case Y_neg:
            position.y -= QUANTUM_DIST;
            break;
    }
}

// Fait avancer le robot... For testing purposes.
void Mouvement::deplacement(SensDeplacement sens, double distance/*unit?*/)
{
    int signe;
    temp_measure = 0;
    if(sens == Avancer)
        signe = 1;
    else
        signe = -1;

    
    while(temp_measure < distance)
    {
        motors->motors_on();
        motors->commande_vitesses(signe*VITESSE, signe*VITESSE, signe*VITESSE, signe*VITESSE);
        delay(QUANTUM_TEMPS);
        motors->motors_stop_low_hiz();
        temp_measure += QUANTUM_DIST;
    }
}

// Rotation dans la direction précisée. For testing purposes.
void Mouvement::rotate(SensRotation sens)
{
    switch (sens)
    {
        case Droite:
            motors->motors_on();
            motors->commande_vitesses(0.5, -0.5, 0.5, -0.5);
            delay(DUREE_ROTATION_DROITE);
            motors->motors_stop_low_hiz();
            break;

        case Gauche:
            motors->motors_on();
            motors->commande_vitesses(-0.5, 0.5, -0.5, 0.5);
            delay(DUREE_ROTATION_GAUCHE);
            motors->motors_stop_low_hiz();
            break;
    }
}


