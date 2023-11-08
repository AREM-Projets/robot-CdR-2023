#ifndef MOUVEMENT_H
#define MOUVEMENT_H
// Constantes à modifier selon le test robot

#include "BlocMoteurs.h"
#include "config.h"
#include "ReseauCapteur.h"

enum SensDeplacement
{
    Reculer,
    Avancer,
    Gauche, // vive les roues mécanum 
    Droite
};

enum SensRotation
{
    Trigo,
    Horaire
};

// Classe mouvement pour des déplacements de base
class Mouvement
{
    public:
        Mouvement(BlocMoteurs* _moteurs, ReseauCapteur* ResCapteurs);
        ~Mouvement();

        void deplacement(SensDeplacement sens, double distance/*unit?*/);
        void rotate(SensRotation sens);

        // signe = 1 pour avancer et -1 pour reculer
        void mouvementElementaire(int signe);

  
        BlocMoteurs* motors;
        ReseauCapteur* capteurs;

    private:
    
        double temp_measure; // for simple methods like forward...

       
};

#endif /* MOUVEMENT_H */
