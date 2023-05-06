// Version récupérée sur le code des robots de la coupe 2022

#include "ControleurPID.h"

bool printing = 0;


#define limiter 300

void ControleurPID::setBlocMoteurs(BlocMoteurs* motors)
{
    moteurs = motors;
}

void ControleurPID::PIDBoost(double multiplier)
{
    Pdist *=multiplier;
    Idist *=multiplier;//2;//4;
    Ddist *=multiplier;

}

ControleurPID::ControleurPID(BlocMoteurs *p_moteurs, Odometrie* aodometrie) : objectif(0, 0, 0), start(INIT_X, INIT_Y, INIT_THETA)
{
    _min_update_period = aodometrie->getMin_update_period();

    // On a choisi de contenir ces éléments dans cette classe puisque le reste du code ne doit pas 
    // y avoir accès sans passer par le controleur
    moteurs = p_moteurs;


    odometrie = aodometrie;

    _date_start = millis();

    // Initialisation des dates.
    // Il s'agit de quelque chose d'important pour que la première consigne ne soit
    // pas bizarre à cause d'une initialisation à 0, et donc d'un premier delta t absurde.
    uint32_t date = micros();
    _last_update_consigne = date;

    objectif_atteint = true;
    objectif_is_rotation = false;
   

    // Les coefficients de l'asservissement polaire
    #ifdef ISMIN
    Pdist = 0.05;
    Idist = 0.0001;//4;
    Ddist = 0.0;

    Pdistcap = 0.1;
    Idistcap = 0.0005;//2;//4;
    Ddistcap = 0.0;
    #else
    Pdist = 0.05;
    Idist = 0.0000;//2;//4;
    Ddist = 0.0;

    Pdistcap = 0.2;
    Idistcap = 0.0005;//2;//4;
    Ddistcap = 0.0;
    #endif



    Pcap = 0.2 ;
    Icap = 0.0;//2;//4;
    Dcap = 0.00;


    sum_erreur_polaire.xerr = 0.0;
    sum_erreur_polaire.yerr = 0.0;
    sum_erreur_polaire.terr = 0.0;
    lastx = odometrie->getX();
    lasty = odometrie->getY();

    AccPhaseDone = false;

    TimerAcc = micros();

    C_FD = 0;
    C_FG = 0;
    C_BD = 0;
    C_BG = 0;

    TimerAccLaunched = false;

}

ControleurPID::~ControleurPID()
{
    // Ces objects sont détruits ici mais ils ne sont pas faits pour être recréés plusieurs fois.
    // Il doit donc a priori s'agir de la fin de tout le programme.
    // Dans le cas contraire le bon comportement des périphériques de la carte n'est pas garenti
    delete moteurs;
    delete odometrie;
}

/**
 * @brief A appeler le plus souvent possible. C'est la fonction elle-même qui tient compte de l'horloge système pour mettre à jour
 *          ou non l'odométrie et les consignes asservies des moteurs.
 */
bool ControleurPID::update()
{
    uint32_t date = micros();

    if(!TimerAccLaunched)
    {
        TimerAcc = micros();
        TimerAccLaunched = true;
        moteurs->StepCeil();
    }

    // l'actualisation de la position doit être effectué le plus souvent possible
    

    // l'actualisation des vitesses moteurs se font à intervalles de temps réguliers
    // (actualiser le plus souvent possible n'est pas nécessaire, voir pas forcément désirable)
    if(odometrie->update())
    {
        // Détermination des erreurs par rapport à l'objectif
        _processErreursPolaires();

        // Application des coefficients pour obtenir les consignes de vitesse
        // + Fin de l'objectif si suffisament proche
        if(objectif_is_rotation)
            _processPIDRotation();
        else
            _processPID();


        _applyConsignes();

        _last_update_consigne = date;
    }

    //ALERTE = max(ALERTE - 1, 0);

    if(objectif_atteint)
        return true;
    else
        return false;
}

/**
 * @brief Met à jour l'odométrie du robot (sa position, la vitesse des moteurs et son orientation).Doit être appelé souvent car on fait l'hypothèse d'une 
 *          évolution linéaire des vitesses depuis le dernier appel.
 */


/**
 * @brief Détermine les erreurs polaires entre la position et l'orientation du robot et l'objectif fixé. Si l'objectif est derrière le robot (+/- 90°), alors l'erreur de distance 
 *          devient négative (comme si on échangeait l'avant et l'arrière du robot).
 */
void ControleurPID::_processErreursPolaires()
{
    // La position et l'orientation actuelle du robot
    double x = odometrie->getX();
    double y = odometrie->getY();
    double theta = odometrie->getThetaRadian();


    // La position et l'orientation de l'objectif
    double c_x = objectif.getX();
    double c_y = objectif.getY();
    double c_theta = objectif.getThetaRadian();
   
    double terr = odometrie->deltaAngleTo(theta, c_theta);
    double xerr = c_x -x;
    double yerr = c_y -y;


    
    double x_relative_error = xerr*cos( theta) - yerr*sin( theta);
    double y_relative_error = yerr*cos( theta) + xerr*sin( theta);

    
    

    // safePrintSerial("Error : ");
    // safePrintSerial(xerr);
    // safePrintSerial(" ");
    // safePrintSerial(yerr);
    // safePrintSerial(" ");
    // safePrintSerial(terr);

    // if( abs(x_relative_error) > limiter || abs(y_relative_error) > limiter/2.5 )
    // {
    //     if(abs(x_relative_error) > abs(2.5*y_relative_error))
    //     {

    //         x_relative_error = ((x_relative_error > 0) - (x_relative_error < 0))*limiter;
    //         y_relative_error = y_relative_error * (limiter / abs(x_relative_error));

    //         xerr = x_relative_error*cos( theta ) + y_relative_error*sin( theta);
    //         yerr = y_relative_error*cos( theta) - x_relative_error*sin( theta);
    //         terr = terr * (limiter/abs(xerr));
          


    //     }
    //     else
    //     {
    //         y_relative_error = ((y_relative_error > 0) - (y_relative_error < 0))*limiter/2.5;
    //         x_relative_error = x_relative_error *((limiter/ 2.5)/abs(y_relative_error));

    //         xerr = x_relative_error*cos( theta ) + y_relative_error*sin( theta);
    //         yerr = y_relative_error*cos( theta) - x_relative_error*sin( theta);
    //         terr = terr * (limiter/2.5/abs(y_relative_error));

    //     }
    //     sum_erreur_polaire.xerr = 0.0;
    //     sum_erreur_polaire.yerr = 0.0;
    //     sum_erreur_polaire.terr = 0.0;
    


    // }

    if( abs(xerr) > limiter || abs(yerr) > limiter )
    {
        if(abs(xerr) > abs(yerr))
        {


            yerr = yerr * (limiter / abs(xerr));
            terr = terr * (limiter/abs(xerr));
            xerr = ((xerr > 0) - (xerr < 0))*limiter;
          


        }
        else
        {
            xerr = xerr * (limiter / abs(yerr));
            terr = terr * (limiter/abs(yerr));
            yerr = ((yerr > 0) - (yerr < 0))*limiter;
        }
        sum_erreur_polaire.xerr = 0.0;
        sum_erreur_polaire.yerr = 0.0;
        sum_erreur_polaire.terr = 0.0;
    


    }


    if(abs(terr) > PI/16)
    {
        sum_erreur_polaire.terr = 0.0;
    }
    if(micros() > TimerAcc + 4000000 )
    {
        AccPhaseDone = true;
    }
    else if( AccPhaseDone == false)
    {
        double multiplier = (micros()-TimerAcc)/4000000.0;
    
        xerr = xerr*multiplier;
        yerr = yerr*multiplier;
        terr = terr*multiplier;
        
    }

    



    // Pour éviter que le robot cherche à se retourner lorsqu'une marche arrière est plus adaptée
    

    // Sauvegarde des erreurs calculées
    erreur_polaire.xerr = xerr;
    erreur_polaire.terr = terr;
    erreur_polaire.yerr = yerr;
}


/**
 * @brief Ajoute un nouvel objectif qui écrase l'ancien. La méthode _processPID sera appelé pour corriger la distance et l'orientation vers l'objectif.
 */
void ControleurPID::setObjectifLigne(double x, double y)
{
    // Il ne s'agit pas d'une rotation seule
    objectif_is_rotation = false;

    // Mise à jour de la nouvelle position de départ et du nouvel objectif
    objectif.setPosition(x, y, odometrie->getThetaRadian());
    start.setPosition(odometrie->getX(), odometrie->getY(), odometrie->getThetaRadian());
    
    objectif_atteint = false;

    // Remise à zéro des valeurs qui dépendent des coups précédents de l'asser
    last_erreur_polaire.yerr = 0;
    last_erreur_polaire.xerr = 0;
    last_erreur_polaire.terr = 0;

    sum_erreur_polaire.yerr = 0;
    sum_erreur_polaire.xerr = 0;
    sum_erreur_polaire.terr = 0;
    
    AccPhaseDone = false;
    TimerAcc = micros();
    TimerAccLaunched = false;

    PosOk = 0;

    C_FD = 0;
    C_FG = 0;
    C_BD = 0;
    C_BG = 0;

   
}

/**
 * @brief Ajoute un nouvel objectif qui écrase l'ancien. La méthode _processPIDRotation sera appelé pour corriger la différence avec l'angle visé.
 */
void ControleurPID::setObjectifRotation(double theta)
{
    // Il s'agit d'une rotation seule
    objectif_is_rotation = true;

    // Mise à jour de la nouvelle position de départ et du nouvel objectif
    objectif.setPosition(odometrie->getX(), odometrie->getY(), theta);
    start.setPosition(odometrie->getX(), odometrie->getY(), odometrie->getThetaRadian());

    objectif_atteint = false;

    // Remise à zéro des valeurs qui dépendent des coups précédents de l'asser
    last_erreur_polaire.xerr = 0;
    last_erreur_polaire.yerr = 0;
    last_erreur_polaire.terr = 0;

    sum_erreur_polaire.xerr = 0;
    sum_erreur_polaire.yerr = 0;
    sum_erreur_polaire.terr = 0;

    PauseLineForRotation=0;

    _OscCounter=0;

    AccPhaseDone = false;
    TimerAcc = millis();
    TimerAccLaunched = false;

    C_FD = 0;
    C_FG = 0;
    C_BD = 0;
    C_BG = 0;
}

void ControleurPID::setObjectif(Objectif target)
{
    // Il s'agit d'une rotation seule
    objectif_is_rotation = target.rotation_only;

    // Mise à jour du nouvel objectif
    // if(objectif_is_rotation)
    //     objectif.setPosition(odometrie->getX(), odometrie->getY(), target.theta);
    // else
    //     objectif.setPosition(target.x, target.y, odometrie->getThetaRadian());

    
    objectif.setPosition(target.x, target.y, target.theta);
    

    // Mise à jour de la nouvelle position de départ de l'objectif
    start.setPosition(odometrie->getX(), odometrie->getY(), odometrie->getThetaRadian());

    objectif_atteint = false;

    // Remise à zéro des valeurs qui dépendent des coups précédents de l'asser
    last_erreur_polaire.xerr = 0;
    last_erreur_polaire.yerr = 0;
    last_erreur_polaire.terr = 0;

    sum_erreur_polaire.xerr = 0;
    sum_erreur_polaire.yerr = 0;
    sum_erreur_polaire.terr = 0;

    PauseLineForRotation=0;

    AccPhaseDone = false;
    TimerAcc = micros();
    TimerAccLaunched = false;
    PosOk = 0;

    C_FD = 0;
    C_FG = 0;
    C_BD = 0;
    C_BG = 0;

}

/**
 * @brief Doit être appelé après la mise à jour de l'erreur polaire. Applique la correction P(ID) et en tire une consigne pour chaque moteur. Se charge également de vérifier les condition de réussite de l'objectif défini.
 */
void ControleurPID::_processPID()
{
    double target_xerr = 0;
    double target_yerr = 0;
    double target_terr = 0;

    double xerr = erreur_polaire.xerr;
    double yerr = erreur_polaire.yerr;
    double terr = erreur_polaire.terr;




    sum_erreur_polaire.xerr += xerr;
    sum_erreur_polaire.yerr += yerr;
    sum_erreur_polaire.terr += terr;

    /////////////////////////////////////////////////////////////
    // Application des coefficients

    target_xerr = Pdist * xerr
                + Idist * sum_erreur_polaire.xerr
                + Ddist * (xerr - last_erreur_polaire.xerr);


    target_yerr = Pdist * yerr
                + Idist * sum_erreur_polaire.yerr
                + Ddist * (yerr - last_erreur_polaire.yerr );
  

    target_terr = Pdistcap * terr
                + Idistcap * sum_erreur_polaire.terr
                + Ddistcap * (terr - last_erreur_polaire.terr );

    
        /////////////////////////////////////////////////////////////
    // Sauvegarde de l'erreur polaire calculée pour le coup suivant
    last_erreur_polaire.xerr = xerr;
    last_erreur_polaire.yerr = yerr;
    last_erreur_polaire.terr = terr;


    double theta = odometrie->getThetaRadian();
    double c_theta = objectif.getThetaRadian();
    terr = odometrie->deltaAngleTo(theta, c_theta);

    // safePrintSerial(" error : ");
    // safePrintSerial(" xerr : ");
    // safePrintSerial(xerr);
    // safePrintSerial(" yerr : " );
    // safePrintSerial(yerr);
    // safePrintSerial(" terr : ");
    // safePrintSerial(terr);

    if(abs(xerr) < 5.0 && abs(yerr) < 5.0  && abs(terr)< 15*ERROR_MIN_ANGLE && AccPhaseDone != false)
    {
        if(PosOk++ > 10)
        {
            objectif_atteint = true;
        }
        
    }
    else
    {
        PosOk = 0;
    }




    /////////////////////////////////////////////////////////////
    // Mise à jour des consignes de vitesse pour les moteurs
    odometrie->compute_robot_to_encoders_AbsoluteRef(&target_xerr,&target_yerr,&target_terr,&C_FD,&C_FG,&C_BD,&C_BG);

    _normalizeConsignes();



}

/**
 * @brief Doit être appelé après la mise à jour de l'erreur polaire. Applique la correction P(ID) et en tire une consigne pour chaque moteur. Se charge également de vérifier les condition de réussite de l'objectif défini.
 *         Ne tient compte que de l'erreur par rapport à l'angle visé (le robot reste donc plus ou moins sur place).
 */
void ControleurPID::_processPIDRotation()
{
    double target_terr = 0.0;
    double target_xerr = 0.0;
    double target_yerr = 0.0;
    // double targ

    double terr = erreur_polaire.terr;

    sum_erreur_polaire.terr += terr;

    /////////////////////////////////////////////////////////////
    // Application des coefficients
    target_terr = Pcap * erreur_polaire.terr 
            + Icap * sum_erreur_polaire.terr
            + Dcap * (terr-last_erreur_polaire.terr);

   

    /////////////////////////////////////////////////////////////
    // Si l'angle du robot est suffisament proche de l'angle visé, l'objectif est considéré comme étant atteint.
    if(abs(erreur_polaire.terr) < 5*ERROR_MIN_ANGLE && AccPhaseDone!= false)
    {
        if(PosOk ++ > 20)
        {
            objectif_atteint = true;
        }
        
    }
    else
    {
        PosOk = 0;
    }

    /////////////////////////////////////////////////////////////
    // Mise à jour des consignes de vitesse pour les moteurs

    odometrie->compute_robot_to_encoders_AbsoluteRef(&target_xerr,&target_yerr,&target_terr,&C_FD,&C_FG,&C_BD,&C_BG);


    _normalizeConsignes();

    /////////////////////////////////////////////////////////////
    // Sauvegarde de l'erreur polaire calculée pour le coup suivant
    last_erreur_polaire.terr = terr;
}

/**
 * @brief S'assure de normaliser les consignes en fonction de la plus grande si elle dépasse 1. Le but est d'éviter de couper une
 *          partie de la différence entre les deux consignes si l'une au moins dépasse 1 (on parle ici des valeurs absolues).
 */
void ControleurPID::_normalizeConsignes()
{

    double tabconsignes[4] = {C_FD,C_FG,C_BG,C_BD};
    double max = C_FD;
    for(int i = 0; i<4; i++)
    {
        if(fabs(tabconsignes[i]) > fabs(max)  )
        {
            max = tabconsignes[i];
        }
    }
    if(fabs(max) > 1.0)
    {
        
        C_FD /= fabs(max);
        C_FG /= fabs(max);
        C_BG /= fabs(max);
        C_BD /= fabs(max);
    }

}

/**
 * @brief Doit être appelé après le calcul des consignes moteur. Envoi ces consignes au shield moteurs si l'objectif n'a pas été 
 *          marqué comme atteint.
 */
void ControleurPID::_applyConsignes()
{
    if(!objectif_atteint) //&& ALERTE <= 0)
    {
        // Au cas où les moteurs avait été arrêté avant
        moteurs->motors_on();
        // Les consignes en paramètre doivent êtres normalisées, autrement elles seront simplement coupées par la fonction
        moteurs->commande_vitesses(C_FD,C_FG,C_BD,C_BG);
    }
    else
    {
        // Bloquage des moteurs (par opposition à la "roue libre" avec la méthode motors_stop_hard_hiz)
        moteurs->motors_stop_low_hiz();
    }
}

BlocMoteurs* ControleurPID::getBlocMoteurs()
{
    return moteurs;
}

Odometrie* ControleurPID::getOdometrie()
{
    return odometrie;
}


void ControleurPID::stop_moteurs()
{
    moteurs->motors_stop_low_hiz();
}
