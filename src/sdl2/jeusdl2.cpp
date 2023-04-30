#include"jeusdl2.h"
#include<cassert>
#include<iostream>
#include<string>
#include<unistd.h>
using namespace std;

const int TAILLE_SPRITE = 32;
const int TAILLE_FONT_X= 25000;
const int TAILLE_FONT_Y= 2000;

Image::Image () : m_surface(nullptr), m_texture(nullptr), m_hasChanged(false) {
}

Image::~Image()
{
    SDL_FreeSurface(m_surface);
    SDL_DestroyTexture(m_texture);

    m_surface = nullptr;
    m_texture = nullptr;
    m_hasChanged = false;
}

void Image::LoadFromFile (const char* filename, SDL_Renderer * renderer) {
    m_surface = IMG_Load(filename);
    if (m_surface == nullptr) {
     string nfn =  string("../") + filename;
        cout << "Error: cannot load "<< filename <<". Trying "<<nfn<< endl;
        m_surface = IMG_Load(nfn.c_str());
        if (m_surface == nullptr) {
            nfn = string("../") + nfn;
            m_surface = IMG_Load(nfn.c_str());
        }
    }
    if (m_surface == nullptr) {
        cout<<"Error: cannot load "<< filename << endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Surface * surfaceCorrectPixelFormat = SDL_ConvertSurfaceFormat(m_surface,SDL_PIXELFORMAT_ARGB8888,0);
    SDL_FreeSurface(m_surface);
    m_surface = surfaceCorrectPixelFormat;

    m_texture = SDL_CreateTextureFromSurface(renderer,surfaceCorrectPixelFormat);
    if (m_texture == NULL) {
         cout << "Error: problem to create the texture of "<< filename << endl;
        SDL_Quit();
        exit(1);
    }
    
}

void Image::LoadFromCurrentSurface (SDL_Renderer * renderer) {
    m_texture = SDL_CreateTextureFromSurface(renderer,m_surface);
    if (m_texture == nullptr) {
         cout << "Error: problem to create the texture from surface " << endl;
        SDL_Quit();
        exit(1);
    }
}

void Image::Draw (SDL_Renderer * renderer, int x, int y, int w, int h) {
    int ok;
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = (w<0)?m_surface->w:w;
    r.h = (h<0)?m_surface->h:h;

    if (m_hasChanged) {
        ok = SDL_UpdateTexture(m_texture,nullptr,m_surface->pixels,m_surface->pitch);
        assert(ok == 0);
        m_hasChanged = false;
    }

    ok = SDL_RenderCopy(renderer,m_texture,nullptr,&r);
    assert(ok == 0);
}

SDL_Texture * Image::GetTexture() const {return m_texture;}

void Image::SetSurface(SDL_Surface * surf) {m_surface = surf;}
















//************************************** JEU ***************************************************************





JeuSDL2 ::JeuSDL2() : gami()
{
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    if (TTF_Init() != 0) {
        cout << "Erreur lors de l'initialisation de la SDL_ttf : " << TTF_GetError() <<   endl;
        SDL_Quit();
        exit(1);
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if( !(IMG_Init(imgFlags) & imgFlags)) {
         cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
         cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() <<  endl;
        cout << "No sound !!!" <<   endl;
        //SDL_Quit();exit(1);
        withSound = false;
    }
    else withSound = true;
//////////////////
	int dimx, dimy;
	dimx = gami.GetConstTerrain().GetDimx();
	dimy = gami.GetConstTerrain().GetDimy();
	dimx = dimx * TAILLE_SPRITE;
	dimy = dimy * TAILLE_SPRITE;

    // Creation de la fenetre
    window = SDL_CreateWindow("Golfnic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dimx, dimy, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
         cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl; 
        SDL_Quit(); 
        exit(1);
    }

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    
    im_mur.LoadFromFile("data/mur.jpg",renderer);
    im_font.LoadFromFile("data/fond_1.jpg",renderer);
    im_balle.LoadFromFile("data/balle_s.png",renderer);
    // IMAGES
    gami.GetTerrain().Ouvrir("./data/niveau1");

 // SONS
    
}


JeuSDL2::~JeuSDL2 () {
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}



void JeuSDL2 :: SDL_Aff()
{ 
 const Terrain& ter = gami.GetConstTerrain();
     const Balle& b = gami.GetConstBalle();
    SDL_RenderClear(renderer);
im_font.Draw(renderer,0*TAILLE_SPRITE,0*TAILLE_SPRITE,TAILLE_FONT_X,TAILLE_FONT_Y);
im_balle.Draw(renderer,b.GetX()*TAILLE_SPRITE,b.GetY()*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE);

        for(unsigned int i=0;i<ter.GetDimx();i++)
            {
                for(unsigned int j=0;j<ter.GetDimy();j++)
                    {
                        if(ter.getXY(i,j)!=nullptr)
                        {
                            if(ter.getXY(i,j)->obs==ter.getXY(i,j)->R){im_mur.Draw(renderer,i*TAILLE_SPRITE,j*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE);}
                            if(ter.getXY(i,j)->obs==ter.getXY(i,j)->F){im_trou.Draw(renderer,i*TAILLE_SPRITE,j*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE);}
                            
                        }
                        
                    }
            }
}


void JeuSDL2 :: BoucleJeu()
{
    gami.SetRaf(1);
    SDL_Event events;

    bool quit = false;
    
    //gami.Rejouer(gami.GetTerrain().GetBalle().mouvement)==0
    while(!quit){
        while (SDL_PollEvent(&events)) {
            
                if (events.type == SDL_QUIT) quit = true;
                else if (events.type == SDL_KEYDOWN) {
                    bool jouer = false;
                    switch (events.key.keysym.sym) {
                        // Commandes du 1er joueur
                        case SDLK_o:
                            gami.ChangerRafraichissement('o');
                            break;
                        case SDLK_l:
                            gami.ChangerRafraichissement('l');
                            break;
                        case SDLK_a:
                            Replacer('a');
                            break;
                        case SDLK_z:
                            gami.AngleChoisis('z');
                            break;
                        case SDLK_s:
                            gami.AngleChoisis('s');
                            break;
                        case SDLK_t:
                            gami.GetPuis('t');
                            break;
                        case SDLK_g:
                            gami.GetPuis('g');
                            break;
                        case SDLK_j: gami.Jouer('j');
                            if(gami.Jouer('j')) 
                            {
                                gami.BackMouvBalle(gami.GetBalle());
                                SDL_Aff_Tab();
                            }
                            cout<< "c'est joué";
                            break;
                        case SDLK_q:
                            quit = true;
                            break;
                        default: break;
                    }
                }
            }
            SDL_Aff();
        SDL_RenderPresent(renderer);
        }       
}

void JeuSDL2 :: Replacer(const char touche)
{
     const Terrain& ter = gami.GetConstTerrain();
    if (touche=='a')
    {
        gami.GetBalle().SetX(3);
        gami.GetBalle().SetY(28);
        SDL_RenderClear(renderer);
        im_font.Draw(renderer,0*TAILLE_SPRITE,0*TAILLE_SPRITE,TAILLE_FONT_X,TAILLE_FONT_Y);
        im_balle.Draw(renderer,gami.GetBalle().GetX()*TAILLE_SPRITE,gami.GetBalle().GetY()*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE);
        for(unsigned int i=0;i<ter.GetDimx();i++)
            {
                for(unsigned int j=0;j<ter.GetDimy();j++)
                    {
                        if(ter.getXY(i,j)!=nullptr)
                        {
                             if(ter.getXY(i,j)->obs==ter.getXY(i,j)->R){im_mur.Draw(renderer,i*TAILLE_SPRITE,j*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE);}
                            if(ter.getXY(i,j)->obs==ter.getXY(i,j)->F){im_trou.Draw(renderer,i*TAILLE_SPRITE,j*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE);}
                            
                        }
                    }
            }
        SDL_RenderPresent(renderer);
    }
}


void JeuSDL2 :: SDL_Aff_Tab()
{
    const Terrain& ter = gami.GetConstTerrain();
    cout<<gami.tabPosX.size()<<endl;
    for (int i = 0; i < gami.tabPosX.size(); i=i+gami.GetRaf())
    {
        cout<<i<<endl;
        SDL_RenderClear(renderer);
        im_font.Draw(renderer,0*TAILLE_SPRITE,0*TAILLE_SPRITE,TAILLE_FONT_X,TAILLE_FONT_Y);
        im_balle.Draw(renderer,gami.tabPosX[i]*TAILLE_SPRITE,gami.tabPosY[i]*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE);
        for(unsigned int i=0;i<ter.GetDimx();i++)
            {
            for(unsigned int j=0;j<ter.GetDimy();j++)
            {
                 if(ter.getXY(i,j)!=nullptr)
                        {
                            if(ter.getXY(i,j)->obs==ter.getXY(i,j)->R){im_mur.Draw(renderer,i*TAILLE_SPRITE,j*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE);}
                            if(ter.getXY(i,j)->obs==ter.getXY(i,j)->F){im_trou.Draw(renderer,i*TAILLE_SPRITE,j*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE);}
                            
                        }
            }
            }
        SDL_RenderPresent(renderer);
        usleep(Raf);
    }
    gami.ClearRepartition();
}



//******************************************BackUP*****************************************************
/*
void JeuSDL2 :: TestAll()
{
    Balle b;
    cout<<gami.GetTerrain().GetGravite().Getangle()<<" ";
    cout<<gami.GetTerrain().GetGravite().ConversionAng()<<" ";
    cout<<gami.GetTerrain().GetGravite().RecupA()<<" ";
    cout<<gami.GetTerrain().GetGravite().RecupB()<<" ";
    cout<<gami.GetTerrain().GetGravite().ConversionX()<<" ";
    cout<<gami.GetTerrain().GetGravite().ConversionY()<<" ";
    gami.GetTerrain().GetGravite().Vitesse(b);
    cout<<"("<<b.vitesse.GetX()<<",";
    cout<<b.vitesse.GetY()<<") ; ";
    b.InitMouvement();
    cout<<b.mouvement.GetX()<<",";
    cout<<b.mouvement.GetY()<<"; ";
    gami.GetTerrain().GetGravite().actualiseVecteur(b);
    gami.ActionJoueurVisuelTest45(b);  
    
    
}

void JeuSDL2 :: TestAffichageBalleContinue(Balle& b)
{
     const Terrain& ter = gami.getConstTerrain();
    // const Balle& b = gami.getConstTerrain().getConstBalle();
    SDL_RenderClear(renderer);

    //Pour le faire avec une puissance définie de 5: gami.GetTerrain().GetGravite().SetPow(5);
    gami.GetTerrain().GetGravite().Getpow();
    gami.GetTerrain().GetGravite().AffPR();
    //Pour le faire avec un angle définie de 45:     gami.GetTerrain().GetGravite().SetAng(45);
    gami.GetTerrain().GetGravite().Getangle();
    gami.GetTerrain().GetGravite().AffAng();
    gami.GetTerrain().GetGravite().Vitesse(b);
    b.AffVitesse();
    b.InitMouvement();
    b.AffInitMouvement();
    //for(int t=0;t<=5;t++)
    // while(Rejouer(b.mouvement))
    //{
        for(int i=0;i<=10;i++)
        { 
            SDL_RenderClear(renderer);
        // if(ter.Collision())
       // {
       //     ter.ArrangementTrajectoire();
        //}
            
            b.MoinsHuitMille();
            for (int i = 0; i <= 50; i++)
            {
                gami.GetTerrain().GetGravite().actualiseVecteurV2(b);       
                gami.GetTerrain().ArrangementTrajectoire(b);
                sdlaff();
                SDL_RenderPresent(renderer);
                usleep(1);
                 if (gami.GetTerrain().Collision(b)) 
                {
                    break;
                } 
            }
            // if (gami.GetTerrain().Collision(b)) 
             //   {
                //    break;
              //  } 
            //else
            //{
                gami.GetTerrain().GetGravite().actualiseVecteur(b);
            //}
            
    }

}
void JeuSDL2 :: Bouclejeu()
{     
    SDL_Event events;

    bool quit = false;
    
    //gami.Rejouer(gami.GetTerrain().GetBalle().mouvement)==0
    while(!quit){
        while (SDL_PollEvent(&events)) {
            
                if (events.type == SDL_QUIT) quit = true;
                else if (events.type == SDL_KEYDOWN) {
                    bool jouer = false;
                    switch (events.key.keysym.sym) {
                        // Commandes du 1er joueur
                        case SDLK_z:
                            gami.angleChoisis('z');
                            break;
                        case SDLK_s:
                            gami.angleChoisis('s');
                            break;
                        case SDLK_t:
                            gami.GetPuis('t');
                            break;
                        case SDLK_g:
                            gami.GetPuis('g');
                            break;
                        case SDLK_j: gami.jouer('j');
                            if(gami.jouer('j')) 
                            {
                               gami.ActionJoueur(gami.GetTerrain().GetBalle());
                            }
                            cout<< "c'est joué";
                            break;
                        case SDLK_q:
                            quit = true;
                            break;
                        default: break;
                    }
                }
            }
            sdlaff();
        SDL_RenderPresent(renderer);
        }

        
        
 }

void JeuSDL2 :: BoucleChoixPUI()
   
    {
        
        SDL_Event events;
          bool jouer = false;

     //bool quit = false;
        while(!jouer){
       
        while (SDL_PollEvent(&events)) 
        {
			
            
             if (events.type == SDL_KEYDOWN) 
            {              // Si une touche est enfoncee
                
				switch (events.key.keysym.sym) 
                {
				case SDLK_z:
					gami.angleChoisis('z');    // car Y inverse
					break;
                case SDLK_s:
					gami.angleChoisis('s');    // car Y inverse
					break;
                case SDLK_j:
                    jouer = true;
                    break;
                    
				default: 
                    break;
				}
            } 
        }
        }

}
void JeuSDL2 :: BoucleChoixANG()
   
    {
          SDL_Event events;

     //bool quit = false;
       
       
        while (SDL_PollEvent(&events)) 
        {
			
            
             if (events.type == SDL_KEYDOWN) 
            {              // Si une touche est enfoncee
                
				switch (events.key.keysym.sym) 
                {
				case SDLK_z:
					gami.angleChoisis('z');    // car Y inverse
					break;
                case SDLK_s:
					gami.angleChoisis('s');    // car Y inverse
					break;
                case SDLK_j:
                   
                    break;
                    
				default: 
                    break;
				}
            } 
        }
        }


void JeuSDL2 :: Rafraichissement (const char touche)
{
    int max = 10000;
				switch (touche) 
                {
				case  'o':
					
                      if(Raf<=max){Raf*10; std :: cout<<"Taux de rafraichissement = "<< Raf;}  // car Y inverse
					
                    break;
                
                case 'l':
                     
                     if(Raf>=1){Raf/10;std :: cout<<"angle = "<< Raf;}
                    
                    break;
				
                default: 
                    break;
				}
std::cout<<"le taux de rafraichissement est "<<Raf<<std::endl;


} 
*/       
        





