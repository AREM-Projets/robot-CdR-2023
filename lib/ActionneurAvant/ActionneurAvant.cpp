#include "ActionneurAvant.h"

ActionneurAvant::ActionneurAvant(int entree_pin_horaire, int entree_pin_fermee)
{
    this -> nm_PIN_Horaire = entree_pin_horaire;
    this -> nm_PIN_Trigo = entree_pin_fermee;
}

void ActionneurAvant::serrer()
{
    digitalWrite(nm_PIN_Horaire, LOW); //Securite pour ne pas activer les deux pins de controle en meme temps
    digitalWrite(nm_PIN_Trigo, HIGH);  //Permet au courant d'arriver dans le moteur
    delay(TEMPS_VISSAGE);              //Laisse le temps au moteur de tourner

    digitalWrite(nm_PIN_Trigo, LOW);   //Arrête le moteur
}

void ActionneurAvant::deserrer()
{
    //Idem fonction precedente, dans l'autre sens

    digitalWrite(nm_PIN_Trigo, LOW);    //Evite les erreurs de manip de bonobo
    digitalWrite(nm_PIN_Horaire, HIGH);
    delay(TEMPS_VISSAGE);

    digitalWrite(nm_PIN_Horaire, LOW);
}