#include "Evitement.h"

#define limitecm 30


void Evitement(Ultrasonic avant, Ultrasonic droite, Ultrasonic gauche, Ultrasonic arriere, BlocMoteurs* motors)
{
    unsigned int distance[4] = {avant.read(), droite.read(), gauche.read(), arriere.read()};
    int secu = 5;
    
    for(int i = 0; i<4; i++)
    {
        if((distance[i] != 0) && (distance[i] < limitecm))
        {
            motors->commande_vitesses(0,0,0,0); //Arrête le robot

            //Doit lire 20 fois une distance de proximité satisfaisante avant de redémarrer. 
            //Si une valeur insatisfaisante est captée, le compteur redemarre.
            while(secu < 20)
            {
                distance[0] = avant.read();
                distance[1] = droite.read();
                distance[2] = gauche.read();
                distance[3] = arriere.read();

                if((avant.read() < limitecm) && (droite.read() < limitecm) && (gauche.read() < limitecm) && (arriere.read() < limitecm))
                {
                    secu++;
                }

                else
                {
                    secu = 0;
                }
                
            }

            //A intégrer, aller jusqu'au point
        }
    }
}
