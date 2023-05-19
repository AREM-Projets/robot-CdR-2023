#include "ReseauCapteur.h"

ReseauCapteur::ReseauCapteur(Ultrasonic* AvantDroite, Ultrasonic* AvantGauche)
{
    Avant1 = AvantDroite;
    Avant2 = AvantGauche;
}

uint32_t ReseauCapteur::EvitementTranslation(int signe, BlocMoteurs* motors)
{
    uint32_t stop_time;
    uint32_t start_time;
    uint32_t time_lost = 0;

    uint16_t distance_avant_gauche;
    uint16_t distance_avant_droite;

    // Serial.println("Evitement");
    switch (signe) //Choix d'avant ou arriere
    {
    case 1:
        
        distance_avant_droite = Avant1->read();
        distance_avant_gauche = Avant2->read();

        if (((distance_avant_droite < LIMITE_CM) && (distance_avant_droite != 0)) || ((distance_avant_gauche < LIMITE_CM) && (distance_avant_gauche != 0)))
        {
            //Si l'un des deux capteurs detecte un obstacle à moins de 20cm, le robot s'arrête 
            motors->motors_stop_low_hiz();
            stop_time = millis();
            if(Serial)
                Serial.println("Obstacle detected, robot stopped.");

            int secu = 0; //Verifie si le robot abverse s'est éloigné en vérifiant VERIF fois.
            while(secu < VERIF)
            {
                distance_avant_droite = Avant1->read();
                distance_avant_gauche = Avant2->read();

                if (((distance_avant_droite < LIMITE_CM) && (distance_avant_droite != 0)) || ((distance_avant_gauche < LIMITE_CM) && (distance_avant_gauche != 0)))
                {
                    secu = 0;
                }
                else
                {
                    secu++;
                }
            }
            // On redémarre le moteur 
            motors->motors_on();
            motors->commande_vitesses(signe*VITESSE, signe*VITESSE, signe*VITESSE, signe*VITESSE);
            if(Serial)
                Serial.println("Robot restarted !");
            // On calcule le temps d'arrêt
            start_time = millis();
            time_lost = start_time - stop_time;
        }
        break;
    
    case -1: // ON A PLUS DE CAPTEURS LOL

        // if (((Arriere1.read() < LIMITE_CM) && (Arriere1.read() != 0)) || ((Arriere2.read() < LIMITE_CM) && (Arriere2.read() != 0))) 
        // {
        //     motors->motors_stop_low_hiz();
        //     stop_time = millis();
        //     if(Serial)
        //         Serial.println("Obstacle detected, robot stopped.");

        //     int secu = 0;
        //     while(secu < VERIF)
        //     {
        //         if (((Arriere1.read() > LIMITE_CM) && (Arriere1.read() != 0)) || ((Arriere2.read() > LIMITE_CM) && (Arriere2.read() != 0)))
        //         {
        //             secu++;
        //         }
        //         else
        //         {
        //             secu = 0;
        //         }
        //     }
        //     // On redémarre le moteur 
        //     motors->motors_on();
        //     motors->commande_vitesses(signe*VITESSE, signe*VITESSE, signe*VITESSE, signe*VITESSE);
        //     if(Serial)
        //         Serial.println("Robot restarted !");
        //     // On calcule le temps d'arrêt
        //     start_time = millis();
        //     time_lost = start_time - stop_time;
        // }
        break;
    }
    return time_lost;
}

