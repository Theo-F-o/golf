#include "balle.h"

Balle :: Balle ()
{
    balle.SetX(10);
    balle.SetY(10);
    position (0.0,0.0);
    gravite (0.0,-9.81);
    vitesse (0.0,0.0);
}



int Balle :: GetX()
{
    balle.GetX();
}

int Balle :: GetY()
{
    balle.GetY();
}


bool Balle :: ArrangementTrajectoire()
{
    return ter.Collision();
}