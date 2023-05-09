#include "Evitement.h"

#define LIMITE_CM 20
#define NOMBRE_CAPTEURS 4


void Evitement(Ultrasonic* capteurs, BlocMoteurs* motors)
{
    unsigned int distance[4] = {capteurs[0].read(), capteurs[1].read(), capteurs[2].read(), capteurs[3].read()};
    int secu;
    
    for(int i = 0; i<4; i++)
    {
        if((distance[i] != 0) && (distance[i] < LIMITE_CM))
        {
            motors->motors_stop_low_hiz(); //Arrête le robot

            //Doit lire 10 fois une distance de proximité satisfaisante avant de redémarrer. 
            //Si une valeur insatisfaisante est captée, le compteur redemarre.
            while(secu < 10*NOMBRE_CAPTEURS)
            {
                secu = 0;
                for(int j = 0; j < 4; j++)
                {
                    if(capteurs[j].read() < LIMITE_CM)
                    {
                        secu++;
                    }
                }
                
            }

            motors->motors_on();
            //Attente de l'odométrie pour faire aller le robot jusqu'à un point donner
        }
    }
}
