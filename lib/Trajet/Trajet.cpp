#include "Trajet.h"

Trajet::Trajet(double max_dist)
{
    
    curseur = 0;
    _max_dist = max_dist; // en mm
}

bool Trajet::estVide() const
{
    return objectifs.size() == 0 || curseur == objectifs.size();
}

void Trajet::_addObjectif(double x, double y, double theta, bool rotation_only)
{
    Objectif n_objectif = {x, y, theta, rotation_only};
    objectifs.push_back(n_objectif);
}

void Trajet::addLigne(double x, double y)
{


    _addObjectif(x, y, 0, false);

}

void Trajet::addRotation(double theta)
{
    _addObjectif(0, 0, theta, true);
}

Objectif Trajet::getObjectif() const
{
    if(objectifs.size() > 0 && curseur < objectifs.size())
    {
        return objectifs.at(curseur);
    }
    else
    {
        Objectif resultat_vide = {0, 0, 0, true};
        return resultat_vide;
    }
}

Objectif Trajet::popObjectif()
{
    Objectif obj = getObjectif();
    curseur = min((curseur + 1), (int) objectifs.size());
    return obj;
}

void Trajet::restart()
{
    curseur = 0;
}

void Trajet::clear()
{
    curseur = 0;
    objectifs.clear();
}
