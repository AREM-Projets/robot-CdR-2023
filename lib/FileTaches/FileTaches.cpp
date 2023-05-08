#include "FileTaches.h"

FileTaches::FileTaches()
{
    curseur_taches = 0;
    nb_taches = 0;
}

void FileTaches::addTrajet(double x, double y, double theta, double multiplier)
{
    Tache tache;
    tache.type = TacheType::TRAJET;
    tache.isover = false;

    tache.x = x;
    tache.y = y;
    tache.theta = theta;
    tache.PIDmult = multiplier;

    addTache(tache);
}

int FileTaches::getCurseur()
{
    return curseur_taches;
}

void FileTaches::addRotation(double theta)
{
    Tache tache;
    tache.type = TacheType::ROTATIONONLY;
    tache.isover = false;

    tache.theta = theta;

    addTache(tache);
}

void FileTaches::addWait(int timerMicros, bool resetAcc)
{
    Tache tache;
    tache.type = TacheType::WAIT;
    tache.isover = false;

    tache.timerMicros = timerMicros;

    addTache(tache);
}

void FileTaches::setOdo(double x, double y, double theta)
{
	Tache tache;
    tache.type = TacheType::SET_ODO;

	tache.x = x;
	tache.y = y;
	tache.theta = theta;

    addTache(tache);
}

void FileTaches::recalage( double targetDistX, double targetDistY, double targetTheta, CalibrationType type, int Timer )
{
    Tache tache;
    tache.type = TacheType::RECALAGE;
    tache.isover = false;

    tache.calib = type;
    tache.timerMicros = Timer;
    if(type == X)
    {
        tache.x = targetDistX;
    }
    else if(type == Y)
    {
        tache.y = targetDistX;
    }
    else if(type == XY)
    {
        tache.x = targetDistX;
        tache.y = targetDistY;
    }

    tache.theta = targetTheta;
    
    addTache(tache);


}

void FileTaches::insertAfterTache(Tache tachetoAdd, int pos)
{
    tachetoAdd.isover=false;
    Tache copytable[MAX_TACHE];

    if(pos >= 0 && pos < nb_taches -1 &&  curseur_taches < MAX_TACHE-1 && pos < MAX_TACHE -1 )
    {
        for(int i = 0; i< nb_taches-pos-2; i++)
        {
            copytable[i] = taches[pos+1+i];       
        }
        taches[pos+1] = tachetoAdd;

        for(int i = 0; i < nb_taches-pos-2; i++)
        {
            taches[pos+2+i]=copytable[i];
        }
        nb_taches++;
   }
   else if( pos == nb_taches -1)
   {
       addTache(tachetoAdd);
   }
}


void FileTaches::addTache(Tache tache)
{
    tache.isover = false;
    if(nb_taches < MAX_TACHE) // Limité par la taille du tableau des taches
    {
        taches[nb_taches] = tache;
        nb_taches = min(nb_taches + 1, MAX_TACHE);
    }
}

Tache FileTaches::popTache()
{
    if(nb_taches > 0 && curseur_taches < nb_taches) // S'il y a au moins une tache
    {
        Tache tache = taches[curseur_taches];

        curseur_taches = min(curseur_taches + 1, nb_taches);
        

        return tache;
    }
    else
    {
        Tache tache_vide;
        tache_vide.type = TacheType::VIDE;
        return tache_vide;
    }
}

void FileTaches::clear()
{
    curseur_taches = 0;
    nb_taches = 0;
}

void FileTaches::restart()
{
    curseur_taches = 0;
    for(int i = 0; i < nb_taches; i++)
    {
        taches->isover = false;
    }
}

bool FileTaches::isDone()
{
    return (nb_taches) == curseur_taches;
}
