#include "Trajet.h"

Trajet::Trajet(Mouvement* _mouvement)
{
    mouvement = _mouvement;

    position.x = 0;
    position.y = 0;
    position.orientation = X_pos; // on suppose le robot orienté selon les x positifs au début.
}

Trajet::~Trajet()
{
    delete mouvement;
}

void Trajet::deplacementBase(SensDeplacement sens)
{
    int signe;
    if(sens == Avancer)
        signe = 1;
    else 
        signe = -1;

    motors->motors_on();
    motors->commande_vitesses(signe * VITESSE, signe * VITESSE, signe * VITESSE, signe * VITESSE);
    delay(QUANTUM_TEMPS);
    motors->motors_stop_low_hiz();
    // sous réserve de définition (ajouter orientation dans la même classe) :
    switch (position.orientation)
    {
        case X_pos:
            position.x += signe * QUANTUM_DIST;
            break;

        case Y_pos:
            position.y += signe * QUANTUM_DIST;
            break;

        case X_neg:
            position.x -= signe * QUANTUM_DIST;
            break;

        case Y_neg:
            position.y -= signe * QUANTUM_DIST;
            break;
    }
}

// void Trajet::addLigne(SensDeplacement sens, double distance)
    
void Trajet::faceDirection(Orientation new_orientation)
{
    // nombre de rotations vers la droite nécessaires
    int nb_rotations = (new_orientation - position.orientation) % 4;

    SensRotation dir;

    switch (nb_rotations)
    {
    case 1:
        mouvement->rotate(Droite);
        delay(AUTOMATIC_DELAY);
        break;
    
    case 2:
        mouvement->rotate(Droite);
        delay(AUTOMATIC_DELAY);
        mouvement->rotate(Droite);
        delay(AUTOMATIC_DELAY);
        break;
    
    case 3:
        mouvement->rotate(Gauche);
        delay(AUTOMATIC_DELAY);
        break;
    
    default:
        break;
    }
    // On update l'orientation après le mouvement
    position.orientation = new_orientation;
}





void goToXvalue(double x)
{

}
        void goToYvalue(double y);
        void goToObjective(Objectif objectif);
        void returnToBase(); // go to (0,0) facing x_pos







double Trajet::getX()
{
    return position.x;
}


double Trajet::getY()
{
    return position.y;
}

Orientation Trajet::getOrientation()
{
    return position.orientation;
}

void Trajet::printPosition()
{
    if(Serial)
    {
        Serial.print("x : ");
        Serial.println(position.x);

        Serial.print("y : ");
        Serial.println(position.y);

        Serial.print("Facing : ");
        switch(position.orientation)
        {
            case X_pos:
                Serial.println("Positive x");
                break;

            case Y_pos:
                Serial.println("Positive y");
                break;

            case X_neg:
                Serial.println("Negative x");
                break;

            case Y_neg:
                Serial.println("Negative y");
                break;   
        }
    }
}

