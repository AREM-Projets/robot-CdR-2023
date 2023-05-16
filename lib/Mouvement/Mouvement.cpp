#include "Mouvement.h"


Mouvement::Mouvement(BlocMoteurs* moteurs, ReseauCapteur* ResCapteurs)
{
    // set motors to the specified motor
    motors = moteurs;
<<<<<<< HEAD
    capteurs = ResCapteurs;
=======

    /* Debug print */
    if(Serial)
    {
        Serial.println("Mouvement initialised");
    }
>>>>>>> 678a0c455cbdc3c076445d7874dd5afab277a222
}

Mouvement::~Mouvement()
{
    delete motors;
}

// Fait avancer le robot... For testing purposes.
void Mouvement::deplacement(SensDeplacement sens, double distance/*unit?*/)
{
    int signe;
    int tempsAttente;
    temp_measure = 0;
    if(sens == Avancer)
        signe = 1;
    else
        signe = -1;

    
    while(temp_measure < distance)
    {
        motors->motors_on();
        int time = millis();
        motors->commande_vitesses(signe*VITESSE, signe*VITESSE, signe*VITESSE, signe*VITESSE); //Bouge
        capteurs->EvitementTranslation(signe, motors); //Regarde autour de lui pour reperer un robot adverse
        tempsAttente = (QUANTUM_TEMPS - (millis() - time)); //Attend QUANTUM_TEMPS moins le temps de la mesure
        if(tempsAttente > 0)
        {
            delay(tempsAttente); //N'attends que si l'on s'est arrêté moins de QUANTUM_TEMPS
        }
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


