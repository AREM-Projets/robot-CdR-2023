#include "Calibration.h"

Calibration::Calibration(BlocMoteurs* motors)
{
    moteurs = motors;
    mouvement = new Mouvement(moteurs);
}

Calibration::~Calibration()
{
    delete mouvement;
    delete moteurs;
}

void Calibration::test_rotations() 
{
    // tour complet à droite, retour position initiale
    mouvement->rotate(Droite);
    delay(DUREE_ROTATION_DROITE);
    mouvement->rotate(Droite);
    delay(DUREE_ROTATION_DROITE);
    mouvement->rotate(Droite);
    delay(DUREE_ROTATION_DROITE);
    mouvement->rotate(Droite);
    delay(DUREE_ROTATION_DROITE);

    // tour complet à gauche, retour position initiale
    mouvement->rotate(Gauche);
    delay(DUREE_ROTATION_GAUCHE);
    mouvement->rotate(Gauche);
    delay(DUREE_ROTATION_GAUCHE);
    mouvement->rotate(Gauche);
    delay(DUREE_ROTATION_GAUCHE);
    mouvement->rotate(Gauche);
    delay(DUREE_ROTATION_GAUCHE);

    // demi-tour droite, demi-tour gauche, retour position initiale
    mouvement->rotate(Droite);
    delay(DUREE_ROTATION_DROITE);
    mouvement->rotate(Droite);
    delay(DUREE_ROTATION_DROITE);
    mouvement->rotate(Gauche);
    delay(DUREE_ROTATION_DROITE);
    mouvement->rotate(Gauche);
    delay(DUREE_ROTATION_DROITE);
}

// modifier le 1000 du déplacement selon l'unité normalisée
void Calibration::test_deplacements() 
{
    // aller retour sur >50 cm (vérifier distance et décalage aller/retour)
    mouvement->deplacement(Avancer, 1000);
    delay(AUTOMATIC_DELAY);
    mouvement->deplacement(Reculer, 1000);
    delay(AUTOMATIC_DELAY);
}

void Calibration::test_carre() 
{
    // rotation, départ à gauche
    mouvement->deplacement(Avancer, 1000);
    delay(AUTOMATIC_DELAY);
    mouvement->rotate(Gauche);
    delay(DUREE_ROTATION_DROITE);
    mouvement->deplacement(Avancer, 1000);
    delay(AUTOMATIC_DELAY);
    mouvement->rotate(Gauche);
    delay(DUREE_ROTATION_DROITE);
    mouvement->deplacement(Avancer, 1000);
    delay(AUTOMATIC_DELAY);
    mouvement->rotate(Gauche);
    delay(DUREE_ROTATION_DROITE);
    mouvement->deplacement(Avancer, 1000);
    delay(AUTOMATIC_DELAY);

    // rotation, départ à droite
    mouvement->deplacement(Avancer, 1000);
    delay(AUTOMATIC_DELAY);
    mouvement->rotate(Droite);
    delay(DUREE_ROTATION_DROITE);
    mouvement->deplacement(Avancer, 1000);
    delay(AUTOMATIC_DELAY);
    mouvement->rotate(Droite);
    delay(DUREE_ROTATION_DROITE);
    mouvement->deplacement(Avancer, 1000);
    delay(AUTOMATIC_DELAY);
    mouvement->rotate(Droite);
    delay(DUREE_ROTATION_DROITE);
    mouvement->deplacement(Avancer, 1000);
    delay(AUTOMATIC_DELAY);
}