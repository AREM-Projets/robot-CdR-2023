// Version récupérée sur le code des robots de la coupe 2022

#ifndef BLOCMOTEURS_H
#define BLOCMOTEURS_H

#include <Arduino.h>
#include "XNucleoIHM02A1.h"
#include "SPI.h"
#include "config.h"

#define RAYON_ROUE 30 // Placeholder, a mesurer // en mm 

#define DISTANCE_ROTATION 100 // distance effectuee par une roue lors d'une rotation, en mm (placeholder)


class BlocMoteurs
{
	public:
		// Constructeur
		BlocMoteurs(SPIClass* spiMotors);
		~BlocMoteurs();
		
		// Pour envoyer une consigne de vitesse aux moteurs
		// les paramètres sont des flottants entre -1 et 1
		// (si la vitesse max est trop faible, il faut modifier la constante MAX_VITESSE dans Reglages.h)
		// (ne pas oublier d'utiliser la méthode motors_on si les moteurs ont été arrêtés avant)
		void commande_vitesses(float vitesse_normalisee_FD, float vitesse_normalisee_FG, float vitesse_normalisee_BD, float vitesse_normalisee_BG );

		// Methodes pour bloquer les moteurs
		// (il faut appeler la méthode motors_on pour pouvoir de nouveau envoyer des consignes
		// de vitesse aux moteurs)
		void motors_stop_hard_hiz();
		void motors_stop_low_hiz();
		
		// Methode pour autoriser la rotation des moteurs
		void motors_on();

		void setPourcentMaxSpeed(double p_pourcentMaxSpeed);
		int DefMicroStep(uint8_t stepmode);
		void StepCeil();

		/* Ajouts 2023 */
		void BlocMoteurs::commande_distance(uint32_t distance_mm, int dir_FG, int dir_FD, int dir_BG, int dir_BD);

		void BlocMoteurs::avancer(uint32_t distance_mm);
		void BlocMoteurs::rotation_droite();
		void BlocMoteurs::rotation_gauche();

		void BlocMoteurs::setMaxSpeedMoteurs(int speed_ppm);


	private:
		// Methodes pour définir la tension affectée à chaque moteur
		void set_vitesse_moteur_FG(int vitesse, StepperMotor::direction_t dir); //frontleft
		void set_vitesse_moteur_FD(int vitesse, StepperMotor::direction_t dir); //frontright
		void set_vitesse_moteur_BG(int vitesse, StepperMotor::direction_t dir); //backleft
		void set_vitesse_moteur_BD(int vitesse, StepperMotor::direction_t dir); //backright
		
		bool moteurs_arret;

		// Un coefficient entre 0 et 1.0 pour limiter facilement la vitesse pendant un mouvement particulier
		double pourcentMaxSpeed;

		XNucleoIHM02A1 *x_nucleo_ihm02a1;  // Création d'une entité pour la carte de contôle des pas à pas
		XNucleoIHM02A1 *x_nucleo_ihm02a1_2;  // Création d'une entité pour la carte de contôle des pas à pas
		SPIClass *dev_spi;
		SPIClass *dev_spi2;
		L6470 **motors;
		L6470 **motors_top;					  // Instance des moteurs


		L6470_init_t initShield[L6470DAISYCHAINSIZE] = { // init parameters for the motors
		/* First Motor.G */
		{
			22.0,							   /* Motor supply voltage in V. */
			200,						   /* Min number of steps per revolution for the motor. = 360/1.8° */
			1.0,							   /* Max motor phase voltage in A. /!\ UNUSED - USELESS /!\ */
			3.5,							   /* Max motor phase voltage in V. /!\ UNUSED - USELESS /!\ */
			0,							   /* Motor initial speed [step/s]. Seems logic at 0*/
			1500.0,						   /* Motor acceleration [step/s^2] (comment for infinite acceleration mode) */
			1500.0,						   /* Motor deceleration [step/s^2] (comment for infinite deceleration mode)*/
			3000,						   /* Motor maximum speed [step/s] */
			0.0,						   /* Motor minimum speed [step/s]*/
			1500,						   /* Motor full-step speed threshold [step/s]. Limit microstep -> fullstep */
			5.3,							   /* Holding kval [V]. */
			5.3,							   /* Constant speed kval [V]. */
			5.3,							   /* Acceleration starting kval [V]. */
			5.3,							   /* Deceleration starting kval [V]. */
			269.9268,							   /* Intersect speed for bemf compensation curve slope changing [step/s]. */
			0.00072448,						   /* Start slope [s/step]. */
			0.0016,						   /* Acceleration final slope [s/step]. */
			0.0016,						   /* Deceleration final slope [s/step]. */
			0,							   /* Thermal compensation factor (range [0, 15]). */
			3.5 * 1000 * 1.10,			   /* Ocd threshold [ma] (range [375 ma, 6000 ma]). Calculated with Kval*/
			3.5 * 1000 * 1.00,			   /* Stall threshold [ma] (range [31.25 ma, 4000 ma]). Calculated Kval */
			StepperMotor::STEP_MODE_HALF, /* Step mode selection. */
			0xFF,						   /* Alarm conditions enable. */
			0x2E88						   /* Ic configuration. */
		},

		/* Second Motor. */
		{
			22.0,							   /* Motor supply voltage in V. */
			200,						   /* Min number of steps per revolution for the motor. = 360/1.8° */
			1.0,							   /* Max motor phase voltage in A. /!\ UNUSED - USELESS /!\ */
			3.5,							   /* Max motor phase voltage in V. /!\ UNUSED - USELESS /!\ */
			0,							   /* Motor initial speed [step/s]. */
			1500.0,						   /* Motor acceleration [step/s^2] (comment for infinite acceleration mode). */
			1500.0,						   /* Motor deceleration [step/s^2] (comment for infinite deceleration mode). */
			3000,						   /* Motor maximum speed [step/s]. */
			0.0,						   /* Motor minimum speed [step/s]. */
			1500,						   /* Motor full-step speed threshold [step/s]. Limit microstep -> fullstep */
			5.3,							   /* Holding kval [V]. */
			5.3,							   /* Constant speed kval [V]. */
			5.3,							   /* Acceleration starting kval [V]. */
			5.3,							   /* Deceleration starting kval [V]. */
			269.9268,							   /* Intersect speed for bemf compensation curve slope changing [step/s]. */
			0.00072448,						   /* Start slope [s/step]. */
			0.0016,						   /* Acceleration final slope [s/step]. */
			0.0016,						   /* Deceleration final slope [s/step]. */
			0,							   /* Thermal compensation factor (range [0, 15]). */
			3.5 * 1000 * 1.10,			   /* Ocd threshold [ma] (range [375 ma, 6000 ma]). */
			3.5 * 1000 * 1.00,			   /* Stall threshold [ma] (range [31.25 ma, 4000 ma]). */
			StepperMotor::STEP_MODE_HALF, /* Step mode selection. */
			0xFF,						   /* Alarm conditions enable. */
			0x2E88						   /* Ic configuration. */		
		}};

	L6470_init_t initShieldMot2[L6470DAISYCHAINSIZE] = { // init parameters for the motors
		/* First Motor.G */
		{
			22.0,						    /* Motor supply voltage in V. */
			200,						   /* Min number of steps per revolution for the motor. = 360/1.8° */
			1.0,							   /* Max motor phase voltage in A. /!\ UNUSED - USELESS /!\ */
			3.5,							   /* Max motor phase voltage in V. /!\ UNUSED - USELESS /!\ */
			0,							   /* Motor initial speed [step/s]. Seems logic at 0*/
			1500.0,						   /* Motor acceleration [step/s^2] (comment for infinite acceleration mode).*/
			1500.0,						   /* Motor deceleration [step/s^2] (comment for infinite deceleration mode).*/
			3000,						   /* Motor maximum speed [step/s]. */
			0.0,						   /* Motor minimum speed [step/s].*/
			1500,						   /* Motor full-step speed threshold [step/s]. Limit microstep -> fullstep */
			5.3,							   /* Holding kval [V]. */
			5.3,							   /* Constant speed kval [V]. */
			5.3,							   /* Acceleration starting kval [V]. */
			5.3,							   /* Deceleration starting kval [V]. */
			269.9268,							   /* Intersect speed for bemf compensation curve slope changing [step/s]. */
			0.00072448,						   /* Start slope [s/step]. */
			0.0016,						   /* Acceleration final slope [s/step]. */
			0.0016,						   /* Deceleration final slope [s/step]. */
			0,							   /* Thermal compensation factor (range [0, 15]). */
			3.5 * 1000 * 1.10,			   /* Ocd threshold [ma] (range [375 ma, 6000 ma]). Calculated with Kval*/
			3.5 * 1000 * 1.00,			   /* Stall threshold [ma] (range [31.25 ma, 4000 ma]). Calculated Kval */
			StepperMotor::STEP_MODE_HALF, /* Step mode selection. */
			0xFF,						   /* Alarm conditions enable. */
			0x2E88						   /* Ic configuration. */
		},

		/* Second Motor. */
		{
			22.0,							   /* Motor supply voltage in V. */
			200,						   /* Min number of steps per revolution for the motor. = 360/1.8° */
			1.0,							   /* Max motor phase voltage in A. /!\ UNUSED - USELESS /!\ */
			3.5,							   /* Max motor phase voltage in V. /!\ UNUSED - USELESS /!\ */
			0,							   /* Motor initial speed [step/s]. */
			1500.0,						   /* Motor acceleration [step/s^2] (comment for infinite acceleration mode). */
			1500.0,						   /* Motor deceleration [step/s^2] (comment for infinite deceleration mode). */
			3000,						   /* Motor maximum speed [step/s]. */
			0.0,						   /* Motor minimum speed [step/s]. */
			1500,						   /* Motor full-step speed threshold [step/s]. Limit microstep -> fullstep */
			5.3,							   /* Holding kval [V]. */
			5.3,							   /* Constant speed kval [V]. */
			5.3,							   /* Acceleration starting kval [V]. */
			5.3,							   /* Deceleration starting kval [V]. */
			269.9268,							   /* Intersect speed for bemf compensation curve slope changing [step/s]. */
			0.00072448,						   /* Start slope [s/step]. */
			0.0016,						   /* Acceleration final slope [s/step]. */
			0.0016,						   /* Deceleration final slope [s/step]. */
			0,							   /* Thermal compensation factor (range [0, 15]). */
			3.5 * 1000 * 1.10,			   /* Ocd threshold [ma] (range [375 ma, 6000 ma]). */
			3.5 * 1000 * 1.00,			   /* Stall threshold [ma] (range [31.25 ma, 4000 ma]). */
			StepperMotor::STEP_MODE_HALF, /* Step mode selection. */
			0xFF,						   /* Alarm conditions enable. */
			0x2E88						   /* Ic configuration. */		
		}};

};





#endif