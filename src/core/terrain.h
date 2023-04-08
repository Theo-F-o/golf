
/**
 * @file terrain.h
 * @brief Définition de la classe Terrain pour la simulation d'un terrain de jeu.
 */

#ifndef _TERRAIN_H
#define _TERRAIN_H

#include"obstacle.h"
#include"gravite.h"
#include<cassert>

/**
 * @class Terrain
 * @brief Classe pour la simulation d'un terrain de jeu.
 *
 * Cette classe permet de simuler un terrain de jeu pour une balle en mouvement, en prenant en compte la gravité et les obstacles présents sur le terrain.
 */
class Terrain 
{
private:
    unsigned int DimX; /**< La dimension en X du terrain. */
    unsigned int DimY; /**< La dimension en Y du terrain. */
    Gravite gr; /**< L'objet gravité utilisé pour simuler la gravité sur le terrain. */
    Obstacle * tab[20][20]; /**< Le tableau de pointeurs d'obstacles représentant les obstacles présents sur le terrain. */

public:
    /**
     * @brief Constructeur par défaut.
     * 
     * Initialise les dimensions du terrain à 0 et l'objet gravité avec une gravité de 0.
     */
    Terrain();

    /**
     * @brief Destructeur par défaut.
     */
    ~Terrain();

    /**
     * @brief Ajoute un obstacle sur le terrain.
     *
     * @param xmin La position minimale en X de l'obstacle.
     * @param ymin La position minimale en Y de l'obstacle.
     * @param xmax La position maximale en X de l'obstacle.
     * @param ymax La position maximale en Y de l'obstacle.
     * @param c L'obstacle à ajouter.
     */
    void SetObstacle(unsigned int xmin, unsigned int ymin, unsigned int xmax, unsigned int ymax, const Obstacle& c);

    /**
     * @brief Vérifie si la balle entre en collision avec un obstacle.
     *
     * @return True si la balle entre en collision avec un obstacle, False sinon.
     */
    bool Collision();

    /**
     * @brief Modifie la trajectoire de la balle en prenant en compte les obstacles présents sur le terrain.
     */
    void ArrangementTrajectoire();

    /**
     * @brief Effectue un test de régression pour la classe Terrain.
     */
    void TestRegression();

    /**
     * @brief Renvoie la dimension en X du terrain.
     * 
     * @return La dimension en X du terrain.
     */
    int getDimx() const;

    /**
     * @brief Renvoie la dimension en Y du terrain.
     *
     * @return La dimension en Y du terrain.
     */
    int getDimy() const;

    /**
     * @brief Renvoie l'objet gravité utilisé pour simuler la gravité sur le terrain.
     *
     * @return L'objet gravité utilisé pour simuler la gravité sur le terrain.
     */
    Gravite& getGravite();

    /**
     * @brief Renvoie le pointeur d'obstacle à la position (x, y) du tableau d'obstacles.
     *
     * @param x La position en X de l'obstacle.
     * @param y La position en Y de l'obstacle.
     * @return renvoie l'adresse de la case a (x,y)  en indices
     */
    

     Obstacle* getXY (const int x, const int y) const;

     //Obstacle GetObs(int x, int y)const;

     bool positionValide(Cellule evaluee);//bool

     //bool CollectionItem(Item I,Balle B);

     

     

};

inline Obstacle * Terrain::getXY (const int x, const int y) const {
	assert(x>=0);
	assert(y>=0);
	assert(x<DimX);
	assert(y<DimY);
	return tab[x][y];
}

inline int Terrain :: getDimx()const{return DimX;}

inline int Terrain :: getDimy()const{return DimY;}

inline Gravite& Terrain :: getGravite(){return gr;}


#endif
