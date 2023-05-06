#pragma once

#include <math.h>

/* 2023 : paramètres à changer au fur et à mesure des tests*/

// La periode de l'asser
#define DELTA_T_ASSER_us 10000.0
#define MAXOSC 100
#define NOTRE_ROBOT

// le perimetre de la roue divisé par le nombre de ticks par tour de l'encodeur magnétique
#define DISTANCE_PAR_TICK_G 60.95 * PI / 4096 // mm
#define DISTANCE_PAR_TICK_D 60.95 * PI / 4096 // mm

// L'écart entre les roues motrices
#define ECART_ROUES 261.5625//250.0 // mm

// L'offset entre le tof et les roues lors de l'estimation de la position du robot avec les tofs
#define DISTANCE_TOF_AVANT_ROUE 80.0 // mm

// L'erreur de distance à l'objectif min pour considérer une ligne droite terminée
#define ERROR_MIN_DIST 10 // mm
// L'erreur d'angle à l'objectif min pour considérer une rotation terminée
#define ERROR_MIN_ANGLE PI / 120 // rad

// L'angle max du robot vers son objectif à partir duquel le robot décide de rotationner sur place avant de commencer le déplacement
// hysteresis pour éviter les oscillations bizarre autour du seuil
#define ANGLE_MAX_ROTATION_SEULE PI / 12.0
#define ANGLE_MIN_ROTATION_SEULE PI / 120.0

// Consigne de vitesse max envoyée au shield moteurs
#define MAX_VITESSE 600.0

#define ACCEL_MAX 10.0 // en mm/s²

//#define INIT_X 2750.0//1500
//#define INIT_Y 1095.0
//#define INIT_THETA PI/2.0

// Les valeurs brutes correspondent à un départ, elles sont transformées en INIT_X/Y/THETA en fonction du coté
#define RAW_INIT_X 0//2675.0
#define RAW_INIT_Y 0//1320.0
#define RAW_INIT_THETA 0///PI/2.0

#ifndef BLEU
    #define INIT_X RAW_INIT_X
    #define INIT_Y RAW_INIT_Y
    #define INIT_THETA RAW_INIT_THETA
#else
    #define INIT_X 3000.0 - RAW_INIT_X
    #define INIT_Y RAW_INIT_Y
    #define INIT_THETA PI - RAW_INIT_THETA
#endif

// Des coefficients calculé pour faire environ une ligne droite sans asser
#define COEF_FG 1.0
#define COEF_FD 1.0
#define COEF_BG 1.0
#define COEF_BD 1.0

#define BRASPASCENTRE 0.0

// Coef pour calibrer les tofs utilisés pour la calibration
#define COEF_TOF 1.0

#define MAX_ERREUR_DIST 200.0
#define MAX_ERREUR_I 100.0

#define MAX_CALI_TOFS_ANGLE 20.0
#define MAX_CALI_TOFS_DIST 100.0

extern int timer_start_tirette;

extern volatile bool mpuInterrupt;  

extern void dmpDataReady();

extern bool dmpReady;

extern bool strategie;

#define LIDARNOTONSERIAL

#ifdef LIDARNOTONSERIAL
#define safePrintSerial(x)  Serial.print(x)



#define safePrintSerialln(x) Serial.println(x)
#else
#define safePrintSerial(x)  

#define safePrintSerialln(x)
#endif

#define POSITION_X_CARRE0 667.5
#define POSITION_Y_CARRE0 1600.0
#define ECART2CARRES 185.0
#define OFFSETFROMEDGELIDARDETECTION 100
#define safeOffset 1000.0
#define TIMEOUTCAMERA 1000000

#define SAFEGALLERIE