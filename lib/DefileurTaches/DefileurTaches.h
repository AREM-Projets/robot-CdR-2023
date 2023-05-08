#ifndef DEFILEURTACHES_H
#define DEFILEURTACHES_H

#include "config.h"
#include "FileTaches.h"
#include "ControleurPID.h"
#include "Odometrie.h"
#include "BlocMoteurs.h"
#include "Trajet.h"


#define RECALAGE_SPEED 5.0
#define PIN_SERVO_REPLIQUE A1
#define ANGLE_SERVO_REPLIQUE_FERME 143//87
#define ANGLE_SERVO_REPLIQUE_OUVRE 100


class DefileurTaches
{
    public:

        DefileurTaches(FileTaches *file_taches,  ControleurPID *controleur, Odometrie* odo);
        bool update();
        void restart();
        void transition(Tache new_tache);
        void setBlocMoteurs(BlocMoteurs* motors);

    private:
        FileTaches *_file_taches;
        ControleurPID *_controleur;
        Tache _current_tache;
        Tache _tacheInsert;
        Odometrie* _odometrie;
        BlocMoteurs* _motors;
        int Timer = 0;
        Objectif obj;
        bool currentTacheOver;
        double w1;
        double w2;
        double w3;
        double w4;
        double vx;
        double vy;
        double wz;
        bool goToMesOver;
        char msgRasp[50];
};

#endif
