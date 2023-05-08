#include "Evitement.h"

#define limitecm 20


void Evitement(Ultrasonic avant, Ultrasonic droite, Ultrasonic gauche, Ultrasonic arriere, BlocMoteurs* motors)
{
    unsigned int distance[4] = {avant.read(), droite.read(), gauche.read(), arriere.read()};
    int secu = 5;
    
    for(int i = 0; i<4; i++)
    {
        if((distance[i] != 0) && (distance[i] < limitecm))
        {
            motors->motors_stop_low_hiz(); //Arrête le robot

            //Doit lire 20 fois une distance de proximité satisfaisante avant de redémarrer. 
            //Si une valeur insatisfaisante est captée, le compteur redemarre.
            while(secu < 10)
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

            motors->motors_on();
            //Attente de l'odométrie pour faire aller le robot jusqu'à un point donner
        }
    }
}
