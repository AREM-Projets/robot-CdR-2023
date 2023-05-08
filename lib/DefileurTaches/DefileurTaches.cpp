#include "DefileurTaches.h"



DefileurTaches::DefileurTaches(FileTaches *file_taches,  ControleurPID *controleur, Odometrie* odo)
{
    _file_taches = file_taches;
    _controleur = controleur;
    _odometrie = odo;
    
    Timer = micros();
    currentTacheOver = false;

    _current_tache.type = TacheType::VIDE;
}

void DefileurTaches::setBlocMoteurs(BlocMoteurs* motors)
{
    _motors = motors;
}

bool DefileurTaches::update()
{
    switch(_current_tache.type)
    {

        case(TacheType::TRAJET):

            if(_controleur->update() == true) // est-ce qu'on est a la position desiree ?
            {
                _controleur->PIDBoost(1/_current_tache.PIDmult);       
                transition(_file_taches->popTache());
            }
            else
            {
                _odometrie->handler(); //Nécessaire pour mettre à jour le gyroscope
            }
            break;

        case(TacheType::ROTATIONONLY):

            if(_controleur->update() == true) // update
            {
                // transition
                transition(_file_taches->popTache());
            }
            else
            {
                _odometrie->handler();
            }
            break;

        case(TacheType::WAIT):
    
            if(micros() > Timer + _current_tache.timerMicros)
            {
                transition(_file_taches->popTache());
            }
            else
            {
                _odometrie->update();
                _odometrie->handler();
            }    
            break;

        case(TacheType::RECALAGE):

            if(micros() > Timer + _current_tache.timerMicros)
            {
                _motors->motors_stop_low_hiz();
                _odometrie->setTheta(_current_tache.theta);

                if(_current_tache.calib == CalibrationType::X)
                {
                    _odometrie->setX(_current_tache.x);
                }
                else if( _current_tache.calib == CalibrationType::Y)
                {
                    _odometrie->setY(_current_tache.y);
                }

                transition(_file_taches->popTache());
            }
            else
            {
                _odometrie->update();
                _odometrie->handler();
            }   
            break;

        default:
        
        	transition(_file_taches->popTache());
        	break;

    }

    return _file_taches->isDone() && _current_tache.isover == true ;
}

void DefileurTaches::transition(Tache new_tache) // changement instantane
{
    switch(new_tache.type)
    {
        case TacheType::TRAJET:
        
            obj = {new_tache.x,new_tache.y,new_tache.theta,false};
            _controleur->setObjectif(obj);
            _controleur->PIDBoost(new_tache.PIDmult);
            break;

        case TacheType::ROTATIONONLY:

            _controleur->setObjectifRotation(new_tache.theta);
            break;

        case TacheType::WAIT:

            Timer = micros();
            break;

        case TacheType::RECALAGE:

            if(new_tache.calib = CalibrationType::X)
            {
                if( fabs(_odometrie->getX()) < 375 ||   fabs(_odometrie->getX()) > 2625 )
                {
                    vy = 0;
                    vx = RECALAGE_SPEED;
                    wz = 0;

                    if( fabs(_odometrie->getX() < 375))
                    {
                        vx*=-1;
                    }

                    _odometrie->compute_robot_to_encoders_AbsoluteRef(&vx,&vy,&wz,&w1,&w2,&w3,&w4);
                    _motors->motors_on();
                    _motors->commande_vitesses(w1,w2,w3,w4);
                    Timer = micros();
                }
            }

            else if(new_tache.calib = CalibrationType::Y)
            {
                if( fabs(_odometrie->getY()) < 250 ||  fabs(_odometrie->getY()) > 1750  )
                {
                    vy = RECALAGE_SPEED;
                    vx = 0;
                    wz = 0;

                    if( fabs(_odometrie->getY() < 250))
                    {
                        vy*=-1;
                    }

                    _odometrie->compute_robot_to_encoders_AbsoluteRef(&vx,&vy,&wz,&w1,&w2,&w3,&w4);
                    _motors->motors_on();
                    _motors->commande_vitesses(w1,w2,w3,w4);

                    Timer = micros();
                }
            }

            else if( new_tache.calib = CalibrationType::XY)
            {
                if( (fabs(_odometrie->getX()) < 375 && fabs(_odometrie->getY()) < 250) || ( fabs(_odometrie->getX()) > 2625 && fabs(_odometrie->getY()) < 250 )  )
                {
                    _tacheInsert.calib =X;
                    _tacheInsert.type=RECALAGE;
                    _tacheInsert.type = TacheType::RECALAGE;
                    _tacheInsert.timerMicros = new_tache.timerMicros;
                    _tacheInsert.x = new_tache.x;
                    _tacheInsert.theta = new_tache.theta;

                    _file_taches->insertAfterTache(_tacheInsert,_file_taches->getCurseur()-1);
                    _tacheInsert.calib =Y;
                    _tacheInsert.y =new_tache.y;
                    _file_taches->insertAfterTache(_tacheInsert,_file_taches->getCurseur()-1);
                }
            }
            break;
		
        case TacheType::SET_ODO:

            _odometrie->setX(new_tache.x);
            _odometrie->setY(new_tache.y);
            _odometrie->setTheta(new_tache.theta);
            break;
		
        default :
            break;

    }

    _current_tache = new_tache;
}

void DefileurTaches::restart()
{
    _file_taches->restart();
}
