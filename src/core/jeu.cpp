#include"jeu.h"
#include<iostream>

/*


  //tant que F pas presser, on fait ActionJoueur()
    

    


/*Void Jeu :: Affichage_txt()
{
    int t=0;
    while (t=0)
    {
        Action
    }
    
}*/


//Tant que F pas presser, on fait ActionJoueurVisuel()

bool Jeu :: Rejouer(Vecteur v)
{
    if(v.magnitude()>=0.1)
    {
        return true;
    }
    else return false;
}

/*void Jeu :: ActionJoueurVisuel()
{
    Gravite gr;
    gr.angleChoisis();
    gr.AffAng();
    gr.GetPuis();
    gr.AffPR();
    gr.Vitesse();
    gr.InitMouvement(B);
    while(!Rejouer(B.mouvement,*this))
    {
        gr.actualiseVecteur(*this);
        AffPosition();
    }
    if(ter.Collision())
    {
        ter.ArrangementTrajectoire();
    }
    
   
}*/

void Jeu :: ActionJoueurVisuelTest45()
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






void Jeu :: angleChoisis (const char touche)
{
    float ang = 45;
    bool choixfini = false;
    int max=90;

    while (choixfini) 
    {
				switch (touche) 
                {
				case 'z':
					
                     while (ang<=90){ang=ang+1.0; std :: cout<<"angle="<<ang;};   // car Y inverse
					
                    break;
                
                case 's':
                     
                     while(ang>=0){ang=ang-1.0;std :: cout<<"angle="<<ang;};
               
                case 'q':
                    
                    choixfini = true;
                    
                    break;
				
                default: 
                    break;
				}
            } 
     ter.getGravite().Angle = ang;

} 



void Jeu :: GetPuis (const char touche)
{
    bool choixfini = false;
    double pui;
    int max=90;

    while (choixfini) 
    {
				switch (touche) 
                {
				case 'z':

					while(max>=pui){pui+=0,5; std :: cout<<pui;} 
                      // car Y inverse
					break;

                case 's':

                    while(pui>=0){pui-=0,5;std :: cout<<pui;}

                    break;

                case 'q':

                    choixfini = true;
				}
            } 
    ter.getGravite().Power=pui;
}