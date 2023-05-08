#ifndef FILETACHES_H
#define FILETACHES_H

#include "Trajet.h"
#include "config.h"
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <list>

#define MAX_TACHE 500

#define large_delay 2000
#define short_delay 250
#define medium_delay 650

#define large_delay_us large_delay*1000
#define short_delay_us short_delay*1000
#define medium_delay_us medium_delay*1000


enum TacheType
{
    TRAJET, //POUR EFFECTUER UN TRAJET vers X Y THETA
    ROTATIONONLY, // ROTATION SEUL (deprecated)
    WAIT, // WAIT
	SET_ODO, // reset l'odometrie a la position donnee
    RECALAGE, // RECALAGE POSITION EN FONCANT DANS LES MURS

    VIDE
};

enum CalibrationType
{
    X,
    Y,
    XY
};

struct Tache
{

    TacheType type;  
    double x;
    double y;
    double theta;
    double PIDmult;
    int timerMicros; // Pour une tache de type WAIT

    CalibrationType calib;

    bool isover;
};

class FileTaches
{
    public:
        FileTaches();
        void addTrajet(double x, double y,double theta, double multiplier = 1.0);
        void addRotation(double theta);
        void addWait(int timerMicros, bool resetAcc = false);
        void addTache(Tache tache);
		void setOdo(double x, double y, double theta);
        void insertAfterTache(Tache tachetoAdd, int pos);
        void recalage( double targetDist1, double targetDist2, double targetTheta, CalibrationType type, int Timer = 5000 );
        
        int getCurseur();

        Tache popTache();
        void clear();
        void restart();
        bool isDone();
        Tache taches[MAX_TACHE];

    private:
       int nb_taches; 
       int curseur_taches;

       
};

#endif
