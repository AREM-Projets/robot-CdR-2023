//Version modifié de la librairie Ultrasonic.h trouvable facilement sur l'Internet

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut) {
  trig = trigPin;
  echo = echoPin;
  threePins = trig == echo ? true : false;
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  timeout = timeOut;

  //mesureEnCours = 0; //Ajout personnel, la mesure ne pouvant être en cours dès la construction
  }

unsigned int Ultrasonic::timing() {
  if (threePins)
    pinMode(trig, OUTPUT);

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  if (threePins)
    pinMode(trig, INPUT);
  
  previousMicros = micros();
  while(!digitalRead(echo) && (micros() - previousMicros) <= timeout); // wait for the echo pin HIGH or timeout
  previousMicros = micros();
  while(digitalRead(echo)  && (micros() - previousMicros) <= timeout); // wait for the echo pin LOW or timeout

  return micros() - previousMicros; // duration
}

/*
 * If the unit of measure is not passed as a parameter,
 * sby default, it will return the distance in centimeters.
 * To change the default, replace CM by INC.
 */
unsigned int Ultrasonic::read(uint8_t und) {
  return timing() / und / 2;  //distance by divisor
}

/*
 * This method is too verbal, so, it's deprecated.
 * Use read() instead.
 */
unsigned int Ultrasonic::distanceRead(uint8_t und) {
  return read(und);
}

/*
void Ultrasonic::Emission()
{
  if (!mesureEnCours)
  {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    mesureEnCours = 1;
  }
}

unsigned int Ultrasonic::Reception()
{
  if (!mesureEnCours)
  {
    previousMicros = micros();
    while(!digitalRead(echo) && (micros() - previousMicros) <= timeout); // wait for the echo pin HIGH or timeout
    previousMicros = micros();
    while(digitalRead(echo)  && (micros() - previousMicros) <= timeout); // wait for the echo pin LOW or timeout

    mesureEnCours = 0;
    return micros() - previousMicros; // duration
  }
}
*/