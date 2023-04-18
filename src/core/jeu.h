#ifndef _JEU
#define _JEU
#include"terrain.h"

class Jeu
{

    private:
    Terrain ter;
    

    public:

    Jeu();

    //void Affichage_txt();

    bool Rejouer(Vecteur v);

    void ActionJoueur(Balle& b);
    void ActionJoueurVisuelTest45(Balle& b);

    Terrain& GetTerrain() ;

    const Terrain& getConstTerrain() const; 

    void angleChoisis(const char touche);

    void GetPuis(const char touche);

    bool jouer(const char jouer);
    
    void testRegression();


};

inline  Terrain& Jeu :: GetTerrain() {return ter;}
inline  const Terrain& Jeu :: getConstTerrain() const {return ter;}


#endif
