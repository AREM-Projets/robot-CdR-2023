/**
 * Version récupérée sur le code des robots de la coupe 2021 (kraken et levi)
 * Modifiée pour les roues mecanum (4 roues plutot que 2 originalement)
 **/

#include "Odometrie.h"

#include "extern.h"




Odometrie::Odometrie(uint32_t min_update_period, MagneticEncodeurs* myencodeurs, bool lidar) : _min_update_period(min_update_period), etat(INIT_X, INIT_Y, INIT_THETA)
{
    
    encodeurs = myencodeurs;
    useLidar = lidar;
    euler[0] = 0.0;


    // pour mettre les angles à zéro au début (pas sur que necessaire sur la version mecanom des encodeurs)
    int64_t deltaTicksFD = encodeurs->getDeltaTicks(FD);
    int64_t deltaTicksFG = encodeurs->getDeltaTicks(FG);
    int64_t deltaTicksBD = encodeurs->getDeltaTicks(BD);
    int64_t deltaTicksBG = encodeurs->getDeltaTicks(BG);
    thetaMPU = INIT_THETA;
    thetaEnco = INIT_THETA;



    
    
    encodeurs->setZeroPosition();

    _last_update_etat = micros();
    RayonRoues = RAYON_ROUE;
    L1pL2 = L1 + L2;

    mpuLaunchedOk = false;
    mpuInterrupt = false;
    safePrintSerialln("Initializing I2C devices...");
    accelgyro.initialize();
    safePrintSerialln("Testing device connections...");
    safePrintSerialln(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    safePrintSerialln("Initializing DMP...");
    devStatus = accelgyro.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity

    #ifdef ROBOT_BENNY
    accelgyro.setXAccelOffset(-2230);
    accelgyro.setYAccelOffset(-535);
    accelgyro.setZAccelOffset(1010);
    accelgyro.setXGyroOffset(73);
    accelgyro.setYGyroOffset(-119);
    accelgyro.setZGyroOffset(-174);
    //OFFSETS    -2230,    -535,    1010,      73,    -119,    -174
    #else
    #ifdef ROBOT_ISMIN
    accelgyro.setXAccelOffset(-2346);
    accelgyro.setYAccelOffset(1691);
    accelgyro.setZAccelOffset(2265);
    accelgyro.setXGyroOffset(78);
    accelgyro.setYGyroOffset(-13);
    accelgyro.setZGyroOffset(20);


    
//OFFSETS    -2346,    1691,    2264,      78,     -13,      20



    #endif 

    #endif


    accelgyro.PrintActiveOffsets();

    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        safePrintSerialln("Enabling DMP...");
        accelgyro.setDMPEnabled(true);

        // enable Arduino interrupt detection
        safePrintSerialln("Enabling interrupt detection (Arduino external interrupt 0)...");
	#if defined (PARTICLE)
        attachInterrupt(D2, dmpDataReady, RISING);
	#else
        attachInterrupt(digitalPinToInterrupt(D6), dmpDataReady, RISING);
	#endif
        mpuIntStatus = accelgyro.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        safePrintSerialln("DMP ready! Waiting for first interrupt...");
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = accelgyro.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        safePrintSerial("DMP Initialization failed (code ");
        safePrintSerial(devStatus);
        safePrintSerialln(F(")"));
        //while(1);
    }

    _last_update_MPU = micros();

    lasteuler = 0.0;
    mpuStable = true;
    mpuCountOk = 0;
    mpuCalibDone = false;
    offsetEuler = 0.0;
    _last_update_etat = micros();



}

void Odometrie::handler()
{
    if(micros()-_last_update_etat < _min_update_period - HANDLERDELAY || micros()-_last_update_etat > _min_update_period + HANDLERDELAY/10)
    {

        if(mpuInterrupt)
        {
            // safePrintSerialln(micros()-_last_update_MPU);
            _last_update_MPU = micros();
            fifoCount = accelgyro.getFIFOCount();
            mpuInterrupt = false;
            mpuIntStatus = accelgyro.getIntStatus();
            if(fifoCount >= packetSize)
            {
                
            
                // check for overflow (this should never happen unless our code is too inefficient)
                if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
                    // reset so we can continue cleanly
                    accelgyro.resetFIFO();
                    safePrintSerialln(F("FIFO overflow!"));

                // otherwise, check for DMP data ready interrupt (this should happen frequently)
                } 
                else if ((mpuIntStatus & 0x02) > 0) {
                    // wait for correct available data length, should be a VERY short wait
                    while (fifoCount < packetSize) fifoCount = accelgyro.getFIFOCount();

                    // read a packet from FIFO
                    accelgyro.getFIFOBytes(fifoBuffer, packetSize);
                    
                    // track FIFO count here in case there is > 1 packet available
                    // (this lets us immediately read more without waiting for an interrupt)
                    fifoCount -= packetSize;

                }
                accelgyro.dmpGetQuaternion(&q, fifoBuffer);
                accelgyro.dmpGetEuler(euler, &q);

                if(fabs(deltaAngleTo(euler[0],lasteuler)) > PI/4 && mpuLaunchedOk ==true)
                {
                    mpuStable = false;
                    safePrintSerialln(" ");
                    safePrintSerial("ACHTUNG : ");
                    safePrintSerial(euler[0]);
                    safePrintSerial(" ");
                    safePrintSerialln(lasteuler);
            
                }
 

                if(fabs(deltaAngleTo(euler[0],lasteuler))<0.0001 && mpuLaunchedOk == false)
                {
                    mpuCountOk += 1;
                    if(mpuCountOk > 5)
                    {
                        mpuLaunchedOk = true;
                        offsetEuler = euler[0];
                    }
                    safePrintSerialln(euler[0]);
                }
                else if( mpuLaunchedOk == false)
                {
                    mpuCountOk =0;
                }

                lasteuler = euler[0];



            }
        }
        

    }
}


Odometrie::~Odometrie()
{
    delete encodeurs;
}

// Needs Encodeurs
bool Odometrie::update(bool useMPU)
{
    uint32_t date = micros();
    // Limitation de la fréquence d'update de l'odométrie (pas de limite si _min_update_period = 0)
    if(date - _last_update_etat > _min_update_period)
    {
        // (On suppose les vitesses constantes pendant t, la trajectoire decrite par le robot est alors un cercle)
        // on manipule des entiers le plus possible, c'est pourquoi l'unité est le micrometre

        uint32_t date = micros();

        // position et orientation actuelle du robot
        double x = etat.getX();
        double y = etat.getY();
        double theta = etat.getThetaRadian();

        // nouvelle position et orientation du robot
        double n_x = 0;
        double n_y = 0;
        double n_theta = 0;

        // ///////////////////////////////////////////////////////////////////////////////////////
        // Si cette fonction update est appellée a intervalles réguliers, les distances peuvent
        // aussi être considérées comme homogènes à des vitesses (car dt se simplifie partout).

        // Récuperation des ticks parcourue par chaque encodeur (entiers de 0 à 4095)
            int64_t deltaTicksFD = encodeurs->getDeltaTicks(FD); // en ticks
            int64_t deltaTicksFG = encodeurs->getDeltaTicks(FG);
            int64_t deltaTicksBD = encodeurs->getDeltaTicks(BD);
            int64_t deltaTicksBG = encodeurs->getDeltaTicks(BG);

        double wRoueFDdt = (double)deltaTicksFD * 2.0 * PI / 4096.0; // en rad (doubles de 0.0 à environ 6.2)
        double wRoueFGdt = (double)deltaTicksFG * 2.0 * PI / 4096.0; // en rad
        double wRoueBDdt = (double)deltaTicksBD * 2.0 * PI / 4096.0; // en rad
        double wRoueBGdt = (double)deltaTicksBG * 2.0 * PI / 4096.0; // en rad

        //////////////////////////////////////////////////////////////////////////////////////////
        // calcul du déplacement du robot ////////////////////////////////////////////////////////

        double vxdt = 0.0;
        double vydt = 0.0;
        double wzdt = 0.0;

        // Calcul des déplacements en x et y et theta relativement au robot
        // C'est dans ce calcul qu'interviennent les dimensions des roues et du chassis
        compute_encoders_to_robot(&wRoueFGdt, &wRoueFDdt, &wRoueBGdt, &wRoueBDdt, &vxdt, &vydt, &wzdt);

        // Calcul des déplacements dans le référentiel du terrain

        thetaMPU = deltaAngleTo(euler[0],offsetEuler);
        thetaEnco = theta + wzdt;

        if( mpuLaunchedOk && mpuStable && useMPU)
        {
            n_theta = thetaMPU;
            wzdt = 0;
            n_x = x + vxdt * cos(theta + wzdt/2) + vydt * sin(theta + wzdt/2);
            n_y = y - vxdt * sin(theta + wzdt/2) + vydt * cos(theta + wzdt/2);
        }
        else
        {
            n_theta = thetaEnco;
            n_x = x + vxdt * cos(theta ) + vydt * sin(theta );
            n_y = y - vxdt * sin(theta ) + vydt * cos(theta );
        }

        safePrintSerial("date: ");
        safePrintSerial(date-_last_update_etat);
        safePrintSerial(" th : ");
        safePrintSerialln(n_theta);

        if(useLidar)
        {
            // Serial.print(n_x);
            // Serial.print("_");
            // Serial.print(n_y);
            // Serial.print("_");
            // Serial.println(n_theta,4);
        }
       
        // safePrintSerial(n_x);
        // safePrintSerial(" y : ");
        // safePrintSerial(n_y);
        // safePrintSerial(" Vx : ");
        // safePrintSerial(vxdt);
        // safePrintSerial(" Vy : ");
        // safePrintSerial(vydt);
        ////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////

        // determination et mise à jour des vitesses indépendantes des moteurs (vitesses en mm/s)
        //etat.setVMG(deltaRoueG / ((double)(date - _last_update_etat) / 1000.0 / 1000.0));
        //etat.setVMD(deltaRoueD / ((double)(date - _last_update_etat) / 1000.0 / 1000.0));

        // application de la nouvelle position calculée et mise à jour des vitesses
        etat.versPosition(n_x, n_y, n_theta, date - _last_update_etat);
        // safePrintSerial(" mpustable :");
        // safePrintSerial(mpuStable);
        // safePrintSerial(" Time : ");
        // safePrintSerial(date - _last_update_etat);
        // safePrintSerial(" X :");
        // safePrintSerial(getX());
        // safePrintSerial(" Y :");
        // safePrintSerial(getY());
        // safePrintSerial(" ticks :");
        // safePrintSerial(" ");
        // safePrintSerial(deltaTicksFD);
        // safePrintSerial(" ");
        // safePrintSerial(deltaTicksFG);
        // safePrintSerial(" ");
        // safePrintSerial(deltaTicksBG);
        // safePrintSerial(" ");
        // safePrintSerial(deltaTicksBD);
        // safePrintSerial(" ");
        // safePrintSerial(" theta ");
        // safePrintSerialln(getThetaRadian());
        
        _last_update_etat = date;
        
        return true;
    }
    return false;
}

double Odometrie::getThetaRadian(bool useMPU) const
{
    if( mpuLaunchedOk && mpuStable && useMPU)
    {
        return thetaMPU;
    }
    else
    {
        return thetaEnco;
    }
}

double Odometrie::getX() const
{
    return etat.getX();
}

double Odometrie::getY() const
{
    return etat.getY();
}

double Odometrie::getV() const
{
    return etat.getV();
}

double Odometrie::getVTheta() const
{
    return etat.getVTheta();
}

double Odometrie::deltaAngleTo(double initial, double target) const
{
    return etat.deltaAngleTo(initial, target);
}

void Odometrie::debug()
{
    encodeurs->debugPrint();
}

void Odometrie::setX(double x)
{
    etat.translatePosition(x, etat.getY(), etat.getThetaRadian());
}

void Odometrie::setY(double y)
{
    etat.translatePosition(etat.getX(), y, etat.getThetaRadian());
}

void Odometrie::setTheta(double theta)
{
    etat.translatePosition(etat.getX(), etat.getY(), theta);
    thetaEnco = theta;
    thetaMPU = theta;
    offsetEuler = deltaAngleTo(-euler[0],theta);
    
    
}

void Odometrie::compute_encoders_to_robot(const double *w1, const double *w2, const double *w3, const double *w4, 
                                        double *vx, double *vy, double *wz)
{
    //                                                               |w1|
    // |vx|         |    1            1           1        1     |   |w2|
    // |vy| = R/4 * |    1           -1          -1        1     | x |w3|
    // |wz|         |-1/(L1+L2)  1/(L1+L2)  -1/(L1+L2)  1/(L1+L2)|   |w4|

    *vx = ((double)RayonRoues/4.0) * (*w1 + *w2 + *w3 + *w4);
    *vy = ((double)RayonRoues/4.0) * (*w1 - *w2 - *w3 + *w4);
    *wz = ((double)RayonRoues/(4.0*(L1pL2))) * (-*w1 + *w2 - *w3 + *w4);
}

void Odometrie::compute_robot_to_encoders(const double *vx, const double *vy, const double *wz, 
                                        double *w1, double *w2, double *w3, double *w4)
{
    // |w1|         |1      1     -(L1+L2)|   |vx|
    // |w2|         |1     -1     -(L1+L2)|   |vy|
    // |w3| = 1/R * |1     -1     -(L1+L2)| x |wz|
    // |w4|         |1      1     -(L1+L2)|

    *w1 = (1.0/((double)RayonRoues)) * (*vx - *vy + (L1pL2)*(*wz));
    *w2 = (1.0/((double)RayonRoues)) * (*vx + *vy - (L1pL2)*(*wz));
    *w3 = (1.0/((double)RayonRoues)) * (*vx + *vy + (L1pL2)*(*wz));
    *w4 = (1.0/((double)RayonRoues)) * (*vx - *vy - (L1pL2)*(*wz));
}

void Odometrie::setRayonRoues(double newRayon)
{
    RayonRoues = newRayon;
}
void Odometrie::setL1pL2(double newL1pL2)
{
    L1pL2 = newL1pL2;

}
double Odometrie::getRayonRoues()
{
    return RayonRoues;

}
double Odometrie::getL1pl2()
{
    return L1pL2;
}

uint32_t Odometrie::getMin_update_period()
{
    return _min_update_period;
}

double Odometrie::getVX() const
{
    return etat.getVX();

}
        
double Odometrie::getVY() const
{
    return etat.getVY();
}

void Odometrie::compute_robot_to_encoders_AbsoluteRef( const double *vx,  const double *vy, const double *wz, double *w1, double *w2, double *w3, double *w4)
{

    double VxAbs = *vx*cos( getThetaRadian()) - *vy*sin( getThetaRadian());
    double VyAbs = *vy*cos( getThetaRadian()) + *vx*sin( getThetaRadian());
    // safePrintSerial(" Vx :");
    // safePrintSerial(*vx);
    // safePrintSerial(" Vy : ");
    // safePrintSerial(*vy);
    // safePrintSerial(" Vxabs : ");
    // safePrintSerial(VxAbs);
    // safePrintSerial(" Vyabs : ");
    // safePrintSerialln(VyAbs);
    compute_robot_to_encoders(&VxAbs,&VyAbs,wz,w1,w2,w3,w4);

}



