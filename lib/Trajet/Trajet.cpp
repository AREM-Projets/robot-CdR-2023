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

    // On se déplace
    mouvement->mouvementElementaire(signe);

    // On modifie la position
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

// Face the specified direction   
void Trajet::setOrientation(Orientation new_orientation)
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

// Le robot se positionne dans le bon sens, puis avance/recule jusqu'à la position voulue
void Trajet::goToXvalue(double x, SensDeplacement sens)
{
    // direction du mouvement : vers les X_pos ou X_neg -> signe du résultat
    double direction = x - position.x;

    // On se place sur la bonne orientation
    if(direction >= 0 && sens == Avancer)
        setOrientation(X_pos);
    else if(direction <= 0 && sens == Avancer)
        setOrientation(X_neg);
    else if(direction > 0 && sens == Reculer)
        setOrientation(X_pos);
    else
        setOrientation(X_neg);

    // On se déplace tant qu'on a pas atteint la position voulue (à ERREUR_MAX près)
    while(fabs(x - position.x) > ERREUR_MAX)
        deplacementBase(sens);
}

// Le robot se positionne dans le bon sens, puis avance/recule jusqu'à la position voulue
void Trajet::goToYvalue(double y, SensDeplacement sens)
{
    // direction du mouvement : vers les Y_pos ou Y_neg -> signe du résultat
    double direction = y - position.y;

    // On se place sur la bonne orientation
    if(direction >= 0 && sens == Avancer)
        setOrientation(Y_pos);
    else if(direction <= 0 && sens == Avancer)
        setOrientation(Y_neg);
    else if(direction > 0 && sens == Reculer)
        setOrientation(Y_pos);
    else
        setOrientation(Y_neg);

    // On se déplace tant qu'on a pas atteint la position voulue (à ERREUR_MAX près)
    while(fabs(y - position.y) > ERREUR_MAX)
        deplacementBase(sens);
}

void Trajet::goToObjective(Objectif objectif)
{
    goToXvalue(objectif.x, Avancer);
    goToYvalue(objectif.y, Avancer);
    setOrientation(objectif.orientation);
}

// go to (0,0), face X_pos
void Trajet::returnToBase()
{
    Objectif objectif;

    objectif.x = 0;
    objectif.y = 0;
    objectif.orientation = X_pos;

    goToObjective(objectif);
}

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

            default:
                Serial.println("Feur");
                break;
        }
    }
}

