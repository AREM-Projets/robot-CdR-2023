// Version récupérée sur le code des robots de la coupe 2022

#ifndef CONTROLEURPID
#define CONTROLEURPID

#include <Arduino.h>
#include "config.h"

#include "Odometrie.h"
#include "EtatDynamiqueRobot.h"
#include "Trajet.h"
#include "BlocMoteurs.h"

#ifndef TCA9548A_h
#include "TCA9548A.h"
#endif


struct ErreurPolaire
{
    double xerr;
    double yerr;
    double terr;
};

class ControleurPID
{
    public:
        ControleurPID(BlocMoteurs *p_moteurs, Odometrie* aodometrie);
        ~ControleurPID();
        void setObjectifLigne(double x, double y);
        void setObjectifRotation(double theta);
        void setObjectif(Objectif target);
        bool update();
        void setBlocMoteurs(BlocMoteurs* motors);
        void PIDBoost(double multiplier);

        // Pas le plus propre du monde mais pas encore d'autre solution
        BlocMoteurs* getBlocMoteurs();
        Odometrie* getOdometrie();
        void stop_moteurs();

        int ALERTE;
    private:
        BlocMoteurs *moteurs;
        Odometrie *odometrie;

        double Pdist;
        double Idist;
        double Ddist;

        double Pdistcap;
        double Idistcap;
        double Ddistcap;

        double Pcap;
        double Icap;
        double Dcap;

        ErreurPolaire erreur_polaire;
        ErreurPolaire last_erreur_polaire;
        ErreurPolaire sum_erreur_polaire;
        void _processErreursPolaires();

        double consigneG;
        double consigneD;
        void _processPID();
        void _processPIDRotation();

        void _processProfileVitesse();

        void _normalizeConsignes();

        void _applyConsignes();

        uint32_t _min_update_period;
        bool objectif_atteint;
        bool objectif_is_rotation;
        bool PauseLineForRotation;
        EtatDynamiqueRobot objectif;
        EtatDynamiqueRobot start;

        int PosOk;


        uint32_t _date_start;
        uint32_t _OscCounter;

        uint32_t _last_update_consigne; // il s'agit de la date de la dernière mise à jour de la consigne aux moteurs
        double lastx;
        double lasty;


        double C_FG;
        double C_BG;
        double C_FD;
        double C_BD;

        bool AccPhaseDone;
        int TimerAcc;
        bool TimerAccLaunched;
    
};

#endif
