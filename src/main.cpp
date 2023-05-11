#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#include "Ultrasonic.h"
#include "config.h"
#include "BlocMoteurs.h"
#include "MagneticEncodeurs.h"
#include "Odometrie.h"
#include "ControleurPID.h"
#include "Evitement.h"
#include "Trappe.h"

#define servoPin D15

Ultrasonic capteurs[4] = {(D1,D2), (D3,D4), (D5,D6), (D7,D8)};
BlocMoteurs* motors;

Servo myservo;

// These define's must be placed at the beginning before #include "STM32TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "STM32TimerInterrupt.h"
#include "STM32_ISR_Timer.h"

#define TIMER_INTERVAL_MS         100
#define HW_TIMER_INTERVAL_MS      50

// Depending on the board, you can select STM32 Hardware Timer from TIM1-TIM22
// For example, F767ZI can select Timer from TIM1-TIM14
// If you select a Timer not correctly, you'll get a message from ci[ompiler
// 'TIMxx' was not declared in this scope; did you mean 'TIMyy'? 

// Init STM32 timer TIM2
STM32Timer ITimer(TIM2);

// Init STM32_ISR_Timer
// Each STM32_ISR_Timer can service 16 different ISR-based timers
STM32_ISR_Timer ISR_Timer;

#define TIMER_INTERVAL_0_1S           100L

void TimerHandler()
{
  ISR_Timer.run();
}

// In STM32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething()
{
  Evitement(capteurs, motors);
}


void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(100);

  // configure pin in output/intput mode
  pinMode(D1, OUTPUT); //Trig
  pinMode(D2, INPUT); //Echo
  pinMode(D3, OUTPUT); //Trig
  pinMode(D4, INPUT); //Echo
  pinMode(D5, OUTPUT); //Trig
  pinMode(D6, INPUT); //Echo
  pinMode(D7, OUTPUT); //Trig
  pinMode(D8, INPUT); //Echo

  pinMode(D15, OUTPUT);
  
  // Attach the Servo variable to a pin:
  myservo.attach(servoPin);


  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  ISR_Timer.setInterval(TIMER_INTERVAL_0_1S,  doingSomething);
}


void loop()
{
  ouvrir(myservo);
  delay(4000);

  fermer(myservo);
  delay(4000);
}