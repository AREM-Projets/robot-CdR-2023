#include "Mouvement.h"

Mouvement::Mouvement(BlocMoteurs* moteurs, ReseauCapteur* ResCapteurs)
{
    // set motors to the specified motor
    motors = moteurs;
    capteurs = ResCapteurs;

    /* Debug print */
    if(Serial)
    {
        Serial.println("Mouvement initialised");
    }
}

Mouvement::~Mouvement()
{
    delete motors;
}

// Configuration roues : en X 
void Mouvement::deplacement(SensDeplacement sens, double distance/*unit?*/)
{
    int tempsAttente;
    int lost_time = 0;
    temp_measure = 0;
    
    while(temp_measure < distance)
    {
        motors->motors_on();
        int time_start = millis();
        switch (sens)
        {
        case SensDeplacement::Avancer:
            motors->commande_vitesses(VITESSE, VITESSE, VITESSE, VITESSE);
            break;
        
        case SensDeplacement::Reculer:
            motors->commande_vitesses(-VITESSE, -VITESSE, -VITESSE, -VITESSE);
            break;

        case SensDeplacement::Droite:
            motors->commande_vitesses(VITESSE, -VITESSE, -VITESSE, VITESSE);
            break;
        
        case SensDeplacement::Gauche:
            motors->commande_vitesses(-VITESSE, VITESSE, VITESSE, -VITESSE);
            break;
        default:
            break;
        }

        // lost_time = capteurs->EvitementTranslation(signe, motors); // Regarde autour de lui pour reperer un robot adverse
        
        while(millis() < time_start + lost_time + QUANTUM_TEMPS); // On attend pour avancer la bonne distance
        
        motors->motors_stop_low_hiz();
        temp_measure += QUANTUM_DIST;
    }
    delay(AUTOMATIC_DELAY);
}

/* Unused */
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
    switch(sens)
    {
        case Horaire:
            motors->motors_on();
            motors->commande_vitesses(0.5, -0.5, 0.5, -0.5);
            delay(DUREE_ROTATION_DROITE);
            motors->motors_stop_low_hiz();
            break;

        case Trigo:
            motors->motors_on();
            motors->commande_vitesses(-0.5, 0.5, -0.5, 0.5);
            delay(DUREE_ROTATION_GAUCHE);
            motors->motors_stop_low_hiz();
            break;
    }  
    delay(AUTOMATIC_DELAY);
}

