 // Version récupérée sur le code des robots de la coupe 2022

#include "BlocMoteurs.h"


BlocMoteurs::BlocMoteurs(SPIClass* spiMotors)
{
    moteurs_arret = 0;


    // pinMode(D2, OUTPUT);
    // digitalWrite(D2, HIGH);
    // pinMode(A2, OUTPUT);
    // digitalWrite(A2, HIGH);


    // Initializing Motor Control Expansion Board.
    dev_spi = spiMotors;
    dev_spi2 = dev_spi;

    // dev_spi2 = new SPIClass(D11, D12, D13);
    // dev_spi2->begin();

    //dev_spi = new SPIClass(A4, PC_2 , D9); ://Test SPI2


    x_nucleo_ihm02a1 = new XNucleoIHM02A1(&initShield[Front], &initShield[Back],&initShieldMot2[Front],&initShieldMot2[Back], A4, A3, A5, D5, D4, D7,  A2, D2 , dev_spi2);
    motors = x_nucleo_ihm02a1->get_components();


    pourcentMaxSpeed = 1.0;

    motors[0]->prepare_move(StepperMotor::FWD,1);
    motors[1]->prepare_move(StepperMotor::FWD,1);
    x_nucleo_ihm02a1->perform_prepared_actions(Front);
    motors[0]->prepare_move(StepperMotor::BWD,1);
    motors[1]->prepare_move(StepperMotor::BWD,1);
    x_nucleo_ihm02a1->perform_prepared_actions(Back);
    // motors_top[0]->prepare_move(StepperMotor::BWD,1);
    // motors_top[1]->prepare_move(StepperMotor::BWD,1);
    // x_nucleo_ihm02a1_2->perform_prepared_actions();



}

BlocMoteurs::~BlocMoteurs()
{
    delete dev_spi;
}

void BlocMoteurs::commande_vitesses(float vitesse_normalisee_FD, float vitesse_normalisee_FG, float vitesse_normalisee_BD, float vitesse_normalisee_BG )
{ 
    /////////////////////////////////////////////////////////////
    // Assurance que les parametres sont entre -1 et 1 ////////////////
    vitesse_normalisee_FG = min(1.0f, vitesse_normalisee_FG);
    vitesse_normalisee_FG = max(-1.0f, vitesse_normalisee_FG);

    vitesse_normalisee_FD = min(1.0f, vitesse_normalisee_FD);
    vitesse_normalisee_FD = max(-1.0f, vitesse_normalisee_FD);

    vitesse_normalisee_BG = min(1.0f, vitesse_normalisee_BG);
    vitesse_normalisee_BG = max(-1.0f, vitesse_normalisee_BG);

    vitesse_normalisee_BD = min(1.0f, vitesse_normalisee_BD);
    vitesse_normalisee_BD = max(-1.0f, vitesse_normalisee_BD);
    /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////

    // Détermination du signe des vitesses (logique inversée entre les deux moteurs car placés symétriquement sur le robot et branchement identique)
    StepperMotor::direction_t sens_FG = vitesse_normalisee_FG >= 0.0f ? StepperMotor::FWD : StepperMotor::BWD;
    StepperMotor::direction_t sens_FD = vitesse_normalisee_FD >= 0.0f ? StepperMotor::BWD : StepperMotor::FWD;
    StepperMotor::direction_t sens_BG = vitesse_normalisee_BG >= 0.0f ? StepperMotor::FWD : StepperMotor::BWD;
    StepperMotor::direction_t sens_BD = vitesse_normalisee_BD >= 0.0f ? StepperMotor::BWD : StepperMotor::FWD;


    // Détermination des valeurs de vitesses réelles à envoyer au shield
    int vitesse_FG = abs(vitesse_normalisee_FG * COEF_FG) * MAX_VITESSE;
    int vitesse_FD = abs(vitesse_normalisee_FD * COEF_FD) * MAX_VITESSE;
    int vitesse_BG = abs(vitesse_normalisee_BG * COEF_BG) * MAX_VITESSE;
    int vitesse_BD = abs(vitesse_normalisee_BD * COEF_BD) * MAX_VITESSE;

    /////////////////////////////////////////////////////////////
    // Au cas où le shield a un comportement indésirable lorsque les vitesses 
    // envoyées sont trop proches de 0 (vibration et bruit des moteurs); 
    // il peut valoir mieux arrêter les moteurs qu'envoyer une vitesse de 0.
    //vitesse_G = max(vitesse_G, 10);
    //vitesse_D = max(vitesse_D, 10);
    /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
    
    set_vitesse_moteur_gauche(vitesse_FG * pourcentMaxSpeed, sens_FG);
    set_vitesse_moteur_droit(vitesse_FD * pourcentMaxSpeed, sens_FD);
    x_nucleo_ihm02a1->perform_prepared_actions(Front);
    set_vitesse_moteur_gauche(vitesse_BG * pourcentMaxSpeed, sens_BG);
    set_vitesse_moteur_droit(vitesse_BD * pourcentMaxSpeed, sens_BD);
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

void BlocMoteurs::commande_distance(uint32_t distance_mm, int dir_FG, int dir_FD, int dir_BG, int dir_BD)
{
    /* Sens de rotation */
    StepperMotor::direction_t sens_FG = dir_FG > 0 ? StepperMotor::BWD : StepperMotor::FWD;
    StepperMotor::direction_t sens_FD = dir_FD > 0 ? StepperMotor::FWD : StepperMotor::BWD;
    StepperMotor::direction_t sens_BG = dir_BG > 0 ? StepperMotor::BWD : StepperMotor::FWD;
    StepperMotor::direction_t sens_BD = dir_BD > 0 ? StepperMotor::FWD : StepperMotor::BWD;

    if (!moteurs_arret)
    {
        uint32_t nb_steps = (uint32_t) 100 * distance_mm / (PI * RAYON_ROUE);

        motors[0]->prepare_move(sens_FG,nb_steps);
        motors[1]->prepare_move(sens_FD,nb_steps);
        x_nucleo_ihm02a1->perform_prepared_actions(Front);
        motors[0]->prepare_move(sens_BG,nb_steps);
        motors[1]->prepare_move(sens_BD,nb_steps);
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

