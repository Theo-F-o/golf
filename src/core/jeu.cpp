#include"jeu.h"
#include<iostream>
#include<cassert>

bool Jeu :: Rejouer(Vecteur v)
{
    if(v.magnitude()>=0.1)
    {
        return true;
    }
    else return false;
}

void Jeu :: ActionJoueur(Balle& b)
{
    
    ter.getGravite().Vitesse(b);
    b.InitMouvement();
  
    while(!Rejouer(b.mouvement))
    {
         if(ter.Collision())
        {
            ter.ArrangementTrajectoire();
        }
        ter.getGravite().actualiseVecteur(b);
        
    }
   
}

/*void Jeu :: ActionJoueurVisuelTest45()
{
    
    ter.getGravite().Power=20;
    ter.getGravite().AffPR();
    ter.getGravite().Angle=45;
    ter.getGravite().AffAng();
    ter.getGravite().Vitesse(ter.getGravite().GetBalle());
    ter.getGravite().GetBalle().AffVitesse();
    ter.getGravite().GetBalle().InitMouvement();
    ter.getGravite().GetBalle().AffInitMouvement();
    for(int t=0;t<=5;t++)
   // while(Rejouer(B.mouvement))
    {
         if(ter.Collision())
        {
            ter.ArrangementTrajectoire();
        }
        ter.getGravite().actualiseVecteur(ter.getGravite().GetBalle());
        
    }
}
*/





void Jeu :: angleChoisis (const char touche)
{
    int ang = ter.getGravite().Getangle();
    
    int max=90;

    
				switch (touche) 
                {
				case  'z':
					
                      if(ang<=max){ang++; std :: cout<<"angle = "<< ang;};   // car Y inverse
					
                    break;
                
                case 's':
                     
                     if(ang>=0){ang--;std :: cout<<"angle = "<< ang;}
                    
                    break;
				
                default: 
                    break;
				}
                std :: cout<< "  angle choisie  "<< ang << std::endl;
            
     ter.getGravite().SetAng(ang);

} 



void Jeu :: GetPuis (const char touche)
{
   
    int pui = ter.getGravite().Getpow();
    int max=10;
				switch (touche) 
                {
				case 't':

					if(max>pui){pui++;}else std :: cout<<"max de puissace atteint"<< std ::endl;
                      // car Y inverse
					break;

                case 'g':

                    if(pui>=0){pui--;} else std ::  cout << "min de puissance atteint " << std :: endl;

                    break;
				}

                assert(pui>=0 && pui <=10);
            std :: cout<< "  puissance choisie  "<< pui << std::endl;
    ter.getGravite().SetPow(pui);
}

bool Jeu :: jouer(const char jouer)
{
    if(jouer=='j') return true; else return false;
}

void Jeu :: testRegression()
{
    Jeu gami;
    int ang = gami.GetTerrain().getGravite().Getangle();
    assert(ang<=90 && ang>=0);
    gami.angleChoisis('z');

    assert(gami.GetTerrain().getGravite().Getangle() && gami.GetTerrain().getGravite().Getangle()!=ang);

    gami.GetPuis('t');

    
}
