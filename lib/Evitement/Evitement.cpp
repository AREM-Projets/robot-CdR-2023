#include "Evitement.h"
#include "BlocMoteurs.h"
#include "Ultrasonic.h"
#include "Arduino.h"

#define LIMITE_CM 20
#define VERIF 10

ReseauCapteur::ReseauCapteur(Ultrasonic pinAvant1, Ultrasonic pinAvant2, Ultrasonic pinArriere1, Ultrasonic pinArriere2, Ultrasonic pinGauche, Ultrasonic pinDroite)
    : Avant1(pinAvant1), Avant2(pinAvant2), Arriere1(pinArriere1), Arriere2(pinArriere2), Gauche(pinGauche), Droite(pinDroite)
{
    //Mettre en argument les paires de pin
}

void ReseauCapteur::EvitementTranslation(int signe, BlocMoteurs motors)
{
    switch (signe) //Choix d'avant ou arriere
    {
    case 1:
        
        if (((Avant1.read() < LIMITE_CM) && (Avant1.read() != 0)) || ((Avant2.read() < LIMITE_CM) && (Avant2.read() != 0)))
        {
            //Si l'un des deux capteurs detecte un obstacle à moins de 20cm, le robot s'arrête 
            motors.motors_stop_low_hiz();

            int secu = 0; //Verifie si le robot abverse s'est éloigné en vérifiant VERIF fois.
            while(secu < VERIF)
            {
                if (((Avant1.read() > LIMITE_CM) && (Avant1.read() != 0)) || ((Avant2.read() > LIMITE_CM) && (Avant2.read() != 0)))
                {
                    secu++;
                }
                else
                {
                    secu = 0;
                }
            }

            motors.motors_on(); //Rallume le moteur
        }

        break;
    
    case -1: //Idem pour l'arriere   
        if (((Arriere1.read() < LIMITE_CM) && (Arriere1.read() != 0)) || ((Arriere2.read() < LIMITE_CM) && (Arriere2.read() != 0))) 
        {
            motors.motors_stop_low_hiz();

            int secu = 0;
            while(secu < VERIF)
            {
                if (((Arriere1.read() > LIMITE_CM) && (Arriere1.read() != 0)) || ((Arriere2.read() > LIMITE_CM) && (Arriere2.read() != 0)))
                {
                    secu++;
                }
                else
                {
                    secu = 0;
                }
            }

            motors.motors_on();
        }

        break;
    }
}

void ReseauCapteur::EvitementRotation(int signe, BlocMoteurs motors) //Fonctionne de la même manière pour les rotations
{
    switch (signe)
    {
    case 1:
        if ((Gauche.read() < LIMITE_CM) && (Gauche.read() != 0)) 
        {
            motors.motors_stop_low_hiz();

            int secu = 0;
            while(secu < VERIF)
            {
                if (((Avant1.read() > LIMITE_CM) && (Avant1.read() != 0)) || ((Avant2.read() > LIMITE_CM) && (Avant2.read() != 0)))
                {
                    secu++;
                }
                else
                {
                    secu = 0;
                }
            }

            motors.motors_on();
        }

        break;
    
    case -1: //da facking same
        if ((Droite.read() < LIMITE_CM) && (Droite.read() != 0))
        {
            motors.motors_stop_low_hiz();

            int secu = 0;
            while(secu < VERIF)
            {
                if (((Arriere1.read() > LIMITE_CM) && (Arriere1.read() != 0)) || ((Arriere2.read() > LIMITE_CM) && (Arriere2.read() != 0)))
                {
                    secu++;
                }
                else
                {
                    secu = 0;
                }
            }

            motors.motors_on();
        }

        break;
    }
}