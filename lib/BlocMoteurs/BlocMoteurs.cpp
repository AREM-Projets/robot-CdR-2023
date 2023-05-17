 // Version récupérée sur le code des robots de la coupe 2022

#include "BlocMoteurs.h"


BlocMoteurs::BlocMoteurs(SPIClass* spiMotors)
{
    moteurs_arret = 0;

    // Initializing Motor Control Expansion Board.
    dev_spi = spiMotors;
    dev_spi2 = dev_spi;

    x_nucleo_ihm02a1 = new XNucleoIHM02A1(&initShield[Front], &initShield[Back],&initShieldMot2[Front],&initShieldMot2[Back], A4, A3, A5, D5, D4, D7,  A2, D2 , dev_spi2);
    motors = x_nucleo_ihm02a1->get_components();


    pourcentMaxSpeed = 1.0;

    motors[0]->prepare_move(StepperMotor::FWD,1);
    motors[1]->prepare_move(StepperMotor::FWD,1);
    x_nucleo_ihm02a1->perform_prepared_actions(Front);
    motors[0]->prepare_move(StepperMotor::BWD,1);
    motors[1]->prepare_move(StepperMotor::BWD,1);
    x_nucleo_ihm02a1->perform_prepared_actions(Back);

    /* Debug print */
    if(Serial)
    {
        Serial.println("BlocMoteurs initialised");
    }

}

BlocMoteurs::~BlocMoteurs()
{
    delete dev_spi;
}

void BlocMoteurs::commande_vitesses(float vitesse_normalisee_FR, float vitesse_normalisee_FL, float vitesse_normalisee_BR, float vitesse_normalisee_BL )
{ 
    /////////////////////////////////////////////////////////////
    // Assurance que les parametres sont entre -1 et 1 ////////////////
    vitesse_normalisee_FL = min(1.0f, vitesse_normalisee_FL);
    vitesse_normalisee_FL = max(-1.0f, vitesse_normalisee_FL);

    vitesse_normalisee_FR = min(1.0f, vitesse_normalisee_FR);
    vitesse_normalisee_FR = max(-1.0f, vitesse_normalisee_FR);

    vitesse_normalisee_BL = min(1.0f, vitesse_normalisee_BL);
    vitesse_normalisee_BL = max(-1.0f, vitesse_normalisee_BL);

    vitesse_normalisee_BR = min(1.0f, vitesse_normalisee_BR);
    vitesse_normalisee_BR = max(-1.0f, vitesse_normalisee_BR);
    /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////

    // Détermination du signe des vitesses (logique inversée entre les deux moteurs car placés symétriquement sur le robot et branchement identique)
    StepperMotor::direction_t sens_FL = vitesse_normalisee_FL >= 0.0f ? StepperMotor::FWD : StepperMotor::BWD;
    StepperMotor::direction_t sens_FR = vitesse_normalisee_FR >= 0.0f ? StepperMotor::BWD : StepperMotor::FWD;
    StepperMotor::direction_t sens_BL = vitesse_normalisee_BL >= 0.0f ? StepperMotor::FWD : StepperMotor::BWD;
    StepperMotor::direction_t sens_BR = vitesse_normalisee_BR >= 0.0f ? StepperMotor::BWD : StepperMotor::FWD;


    // Détermination des valeurs de vitesses réelles à envoyer au shield
    int vitesse_FL = abs(vitesse_normalisee_FL * COEF_FL) * MAX_VITESSE;
    int vitesse_FR = abs(vitesse_normalisee_FR * COEF_FR) * MAX_VITESSE;
    int vitesse_BL = abs(vitesse_normalisee_BL * COEF_BL) * MAX_VITESSE;
    int vitesse_BR = abs(vitesse_normalisee_BR * COEF_BR) * MAX_VITESSE;

    /////////////////////////////////////////////////////////////
    // Au cas où le shield a un comportement indésirable lorsque les vitesses 
    // envoyées sont trop proches de 0 (vibration et bruit des moteurs); 
    // il peut valoir mieux arrêter les moteurs qu'envoyer une vitesse de 0.
    //vitesse_G = max(vitesse_G, 10);
    //vitesse_D = max(vitesse_D, 10);
    /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
    
    set_vitesse_moteur_gauche(vitesse_FL * pourcentMaxSpeed, sens_FL);
    set_vitesse_moteur_droit(vitesse_FR * pourcentMaxSpeed, sens_FR);
    x_nucleo_ihm02a1->perform_prepared_actions(Front);
    set_vitesse_moteur_gauche(vitesse_BL * pourcentMaxSpeed, sens_BL);
    set_vitesse_moteur_droit(vitesse_BR * pourcentMaxSpeed, sens_BR);
    x_nucleo_ihm02a1->perform_prepared_actions(Back);
   
}

void BlocMoteurs::set_vitesse_moteur_gauche(int vitesse, StepperMotor::direction_t dir)
{
    // verification de la validité de la vitesse a envoyer

    vitesse = abs(vitesse);
    vitesse = min(vitesse, (int)MAX_VITESSE);
    //vitesse = vitesse > 10 ? vitesse : 0;

    if (!moteurs_arret)
    {
        motors[0]->prepare_run(dir, vitesse);
    }
    else
    {
        motors[0]->prepare_hard_hiz(); // mode haute impédence pour pouvoir déplacer le robot à la main
    }

    

}

void BlocMoteurs::set_vitesse_moteur_droit(int vitesse, StepperMotor::direction_t dir)
{
    // verification de la validité de la vitesse a envoyer
    
    vitesse = abs(vitesse);
    vitesse = min(vitesse, (int)MAX_VITESSE);
    //vitesse = vitesse > 10 ? vitesse : 0;

    if (!moteurs_arret)
    {
        motors[1]->prepare_run(dir, vitesse);
    }
    else
    {
        motors[1]->prepare_hard_hiz(); // mode haute impédence pour pouvoir déplacer le robot à la main
    }

}

int BlocMoteurs::DefMicroStep(uint8_t stepmode)
{
    initShield[0].step_sel = stepmode;
    initShield[1].step_sel = stepmode;

    if (!motors[0]->set_step_mode((StepperMotor::step_mode_t) initShield[0].step_sel)) {
        return false;
    }
    if (!motors[1]->set_step_mode((StepperMotor::step_mode_t) initShield[1].step_sel)) {
        return false;
    }
    if (!motors[0]->set_step_mode((StepperMotor::step_mode_t) initShield[2].step_sel)) {
        return false;
    }
    if (!motors[1]->set_step_mode((StepperMotor::step_mode_t) initShield[3].step_sel)) {
        return false;
    }

    return true;

}


void BlocMoteurs::motors_on() // il faut activer les moteurs pour qu'il puisse recevoir des commandes vitesse.
{
    moteurs_arret = 0;
}

void BlocMoteurs::motors_stop_hard_hiz() // coupe les moteurs et les rends libres.
{
    
    motors[0]->prepare_hard_hiz(); // mode haute impédence pour pouvoir déplacer le robot à la main
    motors[1]->prepare_hard_hiz();
    x_nucleo_ihm02a1->perform_prepared_actions(Front);
    motors[0]->prepare_hard_hiz(); // mode haute impédence pour pouvoir déplacer le robot à la main
    motors[1]->prepare_hard_hiz();
    x_nucleo_ihm02a1->perform_prepared_actions(Back);

    moteurs_arret = 1;
}



void BlocMoteurs::motors_stop_low_hiz()
{
    
    motors[0]->prepare_run(StepperMotor::BWD, 0);
    motors[1]->prepare_run(StepperMotor::BWD, 0);
    x_nucleo_ihm02a1->perform_prepared_actions(0);
    motors[0]->prepare_run(StepperMotor::BWD, 0);
    motors[1]->prepare_run(StepperMotor::BWD, 0);
    x_nucleo_ihm02a1->perform_prepared_actions(1);

    moteurs_arret = 1;
}


void BlocMoteurs::setPourcentMaxSpeed(double p_pourcentMaxSpeed)
{
    // limitation du coefficient entre 0 et 1.0
    pourcentMaxSpeed = max(min(1.0, p_pourcentMaxSpeed), 0.0);
}

void BlocMoteurs::StepCeil()
{
    motors[0]->prepare_move(StepperMotor::FWD,1);
    motors[1]->prepare_move(StepperMotor::FWD,1);
    x_nucleo_ihm02a1->perform_prepared_actions(Front);
    motors[0]->prepare_move(StepperMotor::BWD,1);
    motors[1]->prepare_move(StepperMotor::BWD,1);
    x_nucleo_ihm02a1->perform_prepared_actions(Back);
}

/* Ajouts 2023 : commande par distance */
// Sens : >0 pour avancer, =< 0 pour reculer

void BlocMoteurs::commande_distance(uint32_t distance_mm, int dir_FL, int dir_FR, int dir_BL, int dir_BR)
{
    /* Sens de rotation */
    StepperMotor::direction_t sens_FL = dir_FL > 0 ? StepperMotor::BWD : StepperMotor::FWD;
    StepperMotor::direction_t sens_FR = dir_FR > 0 ? StepperMotor::FWD : StepperMotor::BWD;
    StepperMotor::direction_t sens_BL = dir_BL > 0 ? StepperMotor::BWD : StepperMotor::FWD;
    StepperMotor::direction_t sens_BR = dir_BR > 0 ? StepperMotor::FWD : StepperMotor::BWD;

    if (!moteurs_arret)
    {
        uint32_t nb_steps = (uint32_t) 100 * distance_mm / (PI * RAYON_ROUE);

        motors[0]->prepare_move(sens_FL,nb_steps);
        motors[1]->prepare_move(sens_FR,nb_steps);
        x_nucleo_ihm02a1->perform_prepared_actions(Front);
        motors[0]->prepare_move(sens_BL,nb_steps);
        motors[1]->prepare_move(sens_BR,nb_steps);
        x_nucleo_ihm02a1->perform_prepared_actions(Back);
    }
    else    
    {
        motors[1]->prepare_hard_hiz(); // mode haute impédence pour pouvoir déplacer le robot à la main
    }
}

/* Default should be 3000 I think but AAAAAAAAAAAAAAA */
void BlocMoteurs::setMaxSpeedMoteurs(int speed_pps)
{
    motors[0]->set_max_speed(speed_pps);
    motors[1]->set_max_speed(speed_pps);
}

/* Avance... */
void BlocMoteurs::avancer(uint32_t distance_mm)
{
    commande_distance(distance_mm, 1, 1, 1, 1);
}

/* Tourne de 90° vers la droite */
void BlocMoteurs::rotation_droite()
{
    commande_distance(DISTANCE_ROTATION, -1, 1, -1, 1);
}

/* Tourne de 90° vers la gauche */
void BlocMoteurs::rotation_gauche()
{
    commande_distance(DISTANCE_ROTATION, 1, -1, 1, -1);
}

