#ifndef _JEUSDL
#define _JEUSDL
#include "core/jeu.h"
#include <SDL2/SDL.h>>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include<iostream>


class JeuSDL2
{
    private :
    Jeu Jeu;
    
    SDL_Window * window;
    SDL_Renderer * renderer;

    TTF_Font * font;
    //Image font_im;
    SDL_Color font_color;

    Mix_Chunk * sound;
    bool withSound;


    public:

    

    void BoucleChoixPUI();


    void BoucleChoixANG();

    void SDLinit();

    void Bouclejeu();
    





};


//! \brief Pour gérer une image avec SDL2
class Image {

private:

    SDL_Surface * m_surface;
    SDL_Texture * m_texture;
    bool m_hasChanged;

public:
    Image () ;
    ~Image();
    void loadFromFile (const char* filename, SDL_Renderer * renderer);
    void loadFromCurrentSurface (SDL_Renderer * renderer);
    void draw (SDL_Renderer * renderer, int x, int y, int w=-1, int h=-1);
    SDL_Texture * getTexture() const;
    void setSurface(SDL_Surface * surf);
};




#endif