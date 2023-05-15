#pragma once

#include <math.h>


// --- //
// Bloc Moteurs
// --- //

/* Consigne de vitesse max envoyée au shield moteurs (en pulse per second) */
#define MAX_VITESSE 600.0

/* Coefficients de vitesses des moteurs pour faire une ligne droite */
#define COEF_FG 1.0
#define COEF_FD 1.0
#define COEF_BG 1.0
#define COEF_BD 1.0

// --- //
// Calibration mouvements
// --- //

/* Durée d'un déplacement élémentaire */
#define QUANTUM_TEMPS           100      

/* Distance parcourue par un déplacement élémentaire */
#define QUANTUM_DIST            100      

/* Durée d'une rotation de 90° vers la droite */
#define DUREE_ROTATION_DROITE   1002

/* Durée d'une rotation de 90° vers la gauche */
#define DUREE_ROTATION_GAUCHE   1005

/* Vitesse normalisée des moteurs pendant une rotation (entre 0 et 1) */
#define VITESSE_ROTATION        0.4

/* Vitesse normalisée des moteurs pendant un déplacement en ligne droite (entre 0 et 1) */
#define VITESSE                 0.4

/* Délai entre deux déplacements */
#define AUTOMATIC_DELAY         500
