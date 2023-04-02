#ifndef _VECTEUR
#define _VECTEUR

class Vecteur
{

    private :
    float x;
    float y;

    public: 
    
     /// @brief constructeur par défaut
     Vecteur();
     /// @brief constructeur par copie 
     Vecteur(float x,float y);
   
   
    /// @brief operateur pour ajouter deeux vecteur
    ///@param v1 vecteur
    
    Vecteur& operator + (const Vecteur &v1);
    
    /// @brief fonction fait la différence entre deux vecteur
    /// @param v1 vecteur
    
    Vecteur& operator - (const Vecteur &v1);

    Vecteur operator / (float scalar)const;

    Vecteur operator * (float scalar)const ;

    Vecteur& operator = (const Vecteur& v);

    ///@brief recupere et retourne les données x et y 

    float GetX()const;

    float GetY()const;

    void SetX(float e);
    void SetY(float f);

    double magnitude()const;

    Vecteur normalized() const;

    double Recupangle()const;
    
    



};





#endif