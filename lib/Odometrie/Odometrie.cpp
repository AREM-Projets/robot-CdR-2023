#include "Odometrie.h"


void Odometrie::faceDirection(Orientation new_orientation)
{
    // nombre de directions et sens (positif vers la droite = sens horaire)
    int nb_rotations = new_orientation - position.orientation;
    SensRotation dir;
    if(nb_rotations > 0)
        dir = Droite;
    else
        dir = Gauche;

    for(int i = 0; i<nb_rotations; i++)
    {
        mouvement->rotate(Droite);
    }
}