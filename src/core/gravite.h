#ifndef _GRAVITE
#define _GRAVITE

#include"balle.h"

class Gravite
{

    private:
   

    public:
    
    //float g = -9.81;

    double Power; 

    double Angle;

    Gravite();

    void actualiseVecteur(Balle& b);

    

    double RecupA();

    double RecupB();
    double ConversionAng();

    double ConversionX();

    double ConversionY();

    Vecteur& Vitesse(Balle& b);

    

    /*void accelerationBalle();*/

    void AffAng();

    void AffPR();
    

    

    void TestRegression();


};









#endif