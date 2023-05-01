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
    //assert(ok == 0);
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
    im_font.LoadFromFile("data/font_1.png",renderer);
    im_balle.LoadFromFile("data/balle_s.png",renderer);
    im_trou.LoadFromFile("./data/trou_1.png",renderer);
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

     Mix_Chunk * wav=Mix_LoadWAV("data/son_jeu.wav");

    if (wav == NULL) {
        std ::cout<<"Impossible de charger le fichier audio : %s\n";
 
    }
    
     int channel = Mix_PlayChannel(-1, wav, 0);
   
    if (channel == -1) {
        printf("Impossible de jouer le fichier audio : %s\n", Mix_GetError());

    }

    Mix_Volume(channel,MIX_MAX_VOLUME/4);

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
                        case SDLK_j: gami.Jouer('j');gami.SScore('j');
                            if(gami.Jouer('j')) 
                            {
                                gami.BackMouvBalle(gami.GetBalle());
                                SDL_Aff_Tab();
                            }
                             cout << "c'est joué";
                            if(gami.Getscore()==0 || gami.GetCoups()==20)
                            {Mix_HaltChannel(channel);
                            Mix_FreeChunk(wav);tab_de_score();}
                            break;
                        case SDLK_q:
                            Mix_HaltChannel(channel);
                            Mix_FreeChunk(wav);
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
        //usleep(Raf);
    }
    gami.ClearRepartition();
}


void JeuSDL2 :: afficherMenu ()
{
    Largeur_fenetre=gami.GetConstTerrain().GetDimx()*TAILLE_SPRITE;
    Hauteur_fenetre=gami.GetConstTerrain().GetDimy()*TAILLE_SPRITE;
    //Ces variables ont servis a positionner les carrées a ne pas effacer, on sait jamais 
    int pos_y= Hauteur_fenetre/2;
    int pos_x= Largeur_fenetre/2;
    int transparence_bouton=175;
    int hauteur_boutton=100;
    int largeur_boutton=100;

    int decale_text=5;

       
    // Variable pour l'ecriture
    TTF_Font * font = TTF_OpenFont("data/pac.ttf", 30);


   
    
    if (font == nullptr)
    {
        // Erreur : impossible de charger la police
        std::cout<<"Erreur init 3"<<endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
 
  //Mise en place du fond
      fond_menu.LoadFromFile("./data/fond_menu.jpg",renderer);
      fond_menu.Draw(renderer,0,0,Largeur_fenetre,Hauteur_fenetre);

      //titre_menu.LoadFromFile("data/coin.png",renderer);
      //titre_menu.Draw(renderer,pos_x,pos_y,Largeur_fenetre/4,Hauteur_fenetre/4);


    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    // Créer un bouton "Jouer"
    jouerButton.x =(pos_x-largeur_boutton/2);
    jouerButton.y = pos_y-hauteur_boutton/2;
    jouerButton.w = largeur_boutton;
    jouerButton.h = hauteur_boutton;
    SDL_SetRenderDrawColor(renderer, 98, 8, 125, transparence_bouton);
    SDL_RenderFillRect(renderer, &jouerButton);
    
    jouerTextSurface = TTF_RenderText_Solid(font, "Jouer", {0, 0, 0});
    jouerTextTexture = SDL_CreateTextureFromSurface(renderer, jouerTextSurface);
    jouerText.x = jouerButton.x+decale_text;
    jouerText.y = jouerButton.y+decale_text;
    jouerText.w = 100;
    jouerText.h = 50;
    SDL_RenderCopy(renderer, jouerTextTexture, nullptr, &jouerText);

    // Créer un bouton "Quitter"
    quitterButton.x =  pos_x-largeur_boutton/2;
    quitterButton.y = (pos_y-hauteur_boutton/2 )+hauteur_boutton*2;
    quitterButton.w = largeur_boutton;
    quitterButton.h = hauteur_boutton;
    SDL_SetRenderDrawColor(renderer, 98,8,125,transparence_bouton);
    SDL_RenderFillRect(renderer, &quitterButton);
    
    quitterTextSurface = TTF_RenderText_Solid(font, "Quitter", {0, 0, 0});
    quitterTextTexture = SDL_CreateTextureFromSurface(renderer, quitterTextSurface);
    quitterText.x =quitterButton.x+decale_text;
    quitterText.y = quitterButton.y+decale_text;
    quitterText.w =largeur_boutton;
    quitterText.h = hauteur_boutton;
    SDL_RenderCopy(renderer, quitterTextTexture, nullptr, &quitterText);


    
    // Afficher le menu
  
    TTF_CloseFont(font);
    SDL_RenderPresent(renderer);

}

void JeuSDL2::Menu(){


    Mix_Chunk * wav=Mix_LoadWAV("data/golfnic_menu.wav");

    if (wav == NULL) {
        std ::cout<<"Impossible de charger le fichier audio : %s\n";
 
    }
    
     int channel = Mix_PlayChannel(-1, wav, 0);
   
    if (channel == -1) {
        printf("Impossible de jouer le fichier audio : %s\n", Mix_GetError());

    }

    Mix_Volume(channel,MIX_MAX_VOLUME/4);

    SDL_Event event;
    bool ouvert=true;
    
    while (ouvert)
    
    {
        afficherMenu();
        
        while(SDL_PollEvent(&event))
        
        {
            switch (event.type)
            
            {
                case SDL_QUIT:
                       
                    destructionFenetre_m();
                    
                    ouvert=false;
                    Mix_HaltChannel(channel);
                    Mix_FreeChunk(wav);
                    break;
            

                case SDL_MOUSEBUTTONDOWN:
                    // L'utilisateur a cliqué sur un bouton
                    
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;   
                    Mix_HaltChannel(channel);
                    Mix_FreeChunk(wav);
                        
                    if (mouseX >= jouerButton.x && mouseX <= jouerButton.x + jouerButton.w && mouseY >= jouerButton.y && mouseY <= jouerButton.y + jouerButton.h)
                    {
                        
                        ouvert=false;

                        //SDL_RenderClear(renderer);
                        //SDL_RenderPresent(renderer);  marche sans mais bizarre 

                        BoucleJeu();
                      
                        break;
                    }
                    
                    else if (mouseX >= quitterButton.x && mouseX <= quitterButton.x + quitterButton.w && mouseY >= quitterButton.y && mouseY <= quitterButton.y + quitterButton.h)
                    {
                        // L'utilisateur a cliqué sur le bouton "Quitter"
                        ouvert=false;
                      
                        destructionFenetre_m();
                        break;
                    }
                    
                   /* else if (mouseX >= didactitielButton.x && mouseX <= didactitielButton.x + didactitielButton.w && mouseY >= didactitielButton.y && mouseY <= didactitielButton.y + didactitielButton.h)
                    {
                        // L'utilisateur a cliqué sur le bouton "Didactitiel"
                        // écrire le code
                   
                        tab_score();
                        ouvert=false;
                     ;
                        break;
                    }*/
                     
             }
            
        }
    }
}
     
void JeuSDL2:: destructionFenetre_m(){

   std :: cout<<"DESTRUCTION :"<<endl;
    
    SDL_DestroyTexture(jouerTextTexture);
   std :: cout<<"1 bouge "<<endl;
    SDL_FreeSurface(jouerTextSurface);
    std ::cout<<"2 bouge "<<endl;

    SDL_DestroyTexture(quitterTextTexture);
    std ::cout<<"3 bouge "<<endl;

    SDL_FreeSurface(quitterTextSurface);

    std ::cout<<"4 bouge "<<endl;

    
    
    SDL_DestroyRenderer(renderer);
    std ::cout<<"rendu bouge "<<endl;

    SDL_DestroyWindow(window);

    std ::cout<<"fenetre bouge "<<endl;

    //SDL_DestroyTexture(Texture_Fenetre);
    std :: cout<<"TEXTURE FENETRE DETRUITE"<<endl;

    //SDL_FreeSurface(Image_Fenetre);
    std :: cout<<"SURFACE IMAGE DETRUITE"<<endl; 
    
    //IMG_Quit();
    SDL_Quit();
    //TTF_Quit();
    
    
}

void JeuSDL2::affiche_game_over()
{
    SDL_SetRenderDrawColor(renderer, 230, 240, 255, 255);
    SDL_RenderClear(renderer);
    int dimx=gami.GetConstTerrain().GetDimx()*TAILLE_SPRITE;
    int dimy=gami.GetConstTerrain().GetDimy()*TAILLE_SPRITE;
SDL_Rect squareRect;
  squareRect.w = 600;
  squareRect.h = 630;
  squareRect.x = (dimx/2-squareRect.w/2);
  squareRect.y = (dimy/2-squareRect.w/2);

  SDL_SetRenderDrawColor(renderer, 98, 8, 125, 175);
  SDL_RenderFillRect(renderer, &squareRect);

  TTF_Font* font1 = TTF_OpenFont("data/pac.ttf", 100);
  SDL_Color textColor = { 0, 0, 0 }; 

  SDL_Surface* textSurface = TTF_RenderText_Solid(font1, "SCORE :", textColor);
  SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
 
  SDL_Rect textRect;
  textRect.w = 100;
  textRect.h = 50;
  textRect.x = squareRect.x  + squareRect.w/4;
  textRect.y = squareRect.y + squareRect.w/4;
  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
 
  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
  char score_str[32];
  cout<<gami.Getscore()<<endl;
  sprintf(score_str, " %d", SDL_GetTicks()/1000);


  textSurface = TTF_RenderText_Solid(font1,score_str, textColor);
  textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  SDL_Rect numberRect;
  
  numberRect.w = textSurface->w;
  numberRect.h = textSurface->h;
  numberRect.x = squareRect.x  + squareRect.w/4;
  numberRect.y =squareRect.y + squareRect.w/2;
  
  SDL_RenderCopy(renderer, textTexture, NULL, &numberRect);
  
  SDL_SetRenderDrawColor(renderer, 125, 190, 131, 175);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
  //Le nombre d'etoile changera en fonction du score
  //Il faut trouver notre facon de calculer le score aussi 

 
  menu.x=squareRect.x+squareRect.w-200;
  menu.y=squareRect.y+squareRect.w/2;
  menu.h=60;
  menu.w=60;
 
  rejouer.x=menu.x;
  rejouer.y=menu.y+menu.h*2;
  rejouer.h=60;
  rejouer.w=60;
 
  quitter.x=menu.x;
  quitter.y=menu.y+menu.h*4;
  quitter.h=60;
  quitter.w=60;

  textSurface=IMG_Load("data/house.png");
  textTexture=SDL_CreateTextureFromSurface(renderer,textSurface);

  quitter_surface=IMG_Load("data/quitter.png");
  rejouer_surface=IMG_Load("data/rejouer.png");

  quitter_texture=SDL_CreateTextureFromSurface(renderer,quitter_surface);
  rejouer_texture=SDL_CreateTextureFromSurface(renderer,rejouer_surface);

  SDL_FreeSurface(quitter_surface);
  SDL_FreeSurface(rejouer_surface);

  SDL_RenderCopy(renderer,textTexture, NULL, &menu);
  SDL_RenderCopy(renderer,rejouer_texture, NULL, &rejouer);
  SDL_RenderCopy(renderer,quitter_texture, NULL, &quitter);
  
 
  SDL_RenderPresent(renderer);

  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
  SDL_DestroyTexture(quitter_texture);
  SDL_DestroyTexture(rejouer_texture);
  
  TTF_CloseFont(font1);

 


}

void JeuSDL2 ::tab_de_score(){

  Mix_Chunk * wav=Mix_LoadWAV("data/game_over.wav");

  if (wav == NULL) {
        cout<<"Impossible de charger le fichier audio : %s\n";
 
    }
    
  int channel = Mix_PlayChannel(-1, wav, 0);
   
  if (channel == -1) {
        printf("Impossible de jouer le fichier audio : %s\n", Mix_GetError());

    }

  Mix_Volume(channel,MIX_MAX_VOLUME/4);


  // possible dans game_over SDL_RenderCopy(renderer,Texture_Fenetre,NULL,&rect_fenetre);

    affiche_game_over();
    bool ouvert=true;
    SDL_Event e;
    while(ouvert){
        
        while(SDL_PollEvent(&e)){
                
            if(e.type==SDL_MOUSEBUTTONDOWN){
                    
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                Mix_HaltChannel(channel);
                Mix_FreeChunk(wav);
                 
                if(mouseX >= menu.x && mouseX <= menu.x + menu.w && mouseY >= menu.y && mouseY <= menu.y + menu.h){
                    ouvert=false;
                    Menu();
                    break;
                }

                if( mouseX >= rejouer.x && mouseX <= rejouer.x + rejouer.w && mouseY >= rejouer.y && mouseY <= rejouer.y + rejouer.h){
                    
                    ouvert=false;
                    BoucleJeu();
                }

                if( mouseX >= quitter.x && mouseX <= quitter.x +  quitter.w && mouseY >=  quitter.y && mouseY <=  quitter.y +  quitter.h){
                    
                    ouvert=false;
                    destructionFenetre_m();
                    break;
                }

                    //J'ajouterai le code quand je trouverai les images pour recommancer une partie, aller au menu et quitter le jeu
                }
            }
    
    
    }
    
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
        





