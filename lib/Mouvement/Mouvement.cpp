#include "Mouvement.h"


Mouvement::Mouvement(BlocMoteurs* moteurs, ReseauCapteur* ResCapteurs)
{
    // set motors to the specified motor
    motors = moteurs;
    capteurs = ResCapteurs;
}

Mouvement::~Mouvement()
{
    delete motors;
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
        int time = millis();
        motors->commande_vitesses(signe*VITESSE, signe*VITESSE, signe*VITESSE, signe*VITESSE);
        //delay(QUANTUM_TEMPS);
        capteurs->EvitementTranslation(signe, motors);
        motors->motors_stop_low_hiz();
        temp_measure += QUANTUM_DIST;
    }
}


void Mouvement::mouvementElementaire(int signe)
{
    motors->motors_on();
    motors->commande_vitesses(signe*VITESSE, signe*VITESSE, signe*VITESSE, signe*VITESSE);
    delay(QUANTUM_TEMPS);
    motors->motors_stop_low_hiz();
}

// Rotation dans la direction précisée.
void Mouvement::rotate(SensRotation sens)
{
    switch (sens)
    {
        case Droite:
            motors->motors_on();
            motors->commande_vitesses(0.5, -0.5, 0.5, -0.5);
            delay(DUREE_ROTATION);
            motors->motors_stop_low_hiz();
            break;

        case Gauche:
            motors->motors_on();
            motors->commande_vitesses(-0.5, 0.5, -0.5, 0.5);
            delay(DUREE_ROTATION);
            motors->motors_stop_low_hiz();
            break;
    }
}


