#include <iostream>
#include <cstdlib>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <sstream>
using namespace std;
//`sdl-config --libs`
//-lSDLmain -lSDL -lSDL_image -lSDL_ttf

const int SCREEN_WIDTH = 751;
const int SCREEN_HEIGHT = 761;
const int SCREEN_BPP=32;
const int NB_CANARD_TO_WIN = 6;
const int NB_CANARD_PAR_NIVEAU = 10;

SDL_Surface *load_image( std::string filename )
{
    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    //Load the image
    loadedImage = SDL_LoadBMP( filename.c_str() );
    //If nothing went wrong in loading the image
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
    }
    //Return the optimized image
    return optimizedImage;
}

SDL_Surface *loadImageWithColorKey(string filename, int r, int g, int b)
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );

        //If the image was optimized just fine
        if( optimizedImage != NULL )
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, r, g, b );

            //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }


    }
    //Return the optimized image
    return optimizedImage;

}

void applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset );
}

void showMessageScreen(string message,int x,int y, TTF_Font *font,int fontSize,SDL_Color textColor,SDL_Surface* &screen)
{
  string mot="";
  string space=" ";
  int i=0,j;
  SDL_Surface *mes=NULL;

  j = message.find(space);
  while( j != string::npos )
  {
      mot=message.substr(i,j-i);
      if(mot != "")
      {
         mes=TTF_RenderText_Solid(font,mot.c_str(),textColor);
         applySurface(x,y,mes,screen,NULL);
         x+=mes->w;
         SDL_FreeSurface(mes);
      }
      x+=fontSize;
      i=j+1;
      j = message.find(space,i);
  }

  mot=message.substr(i);
  mes=TTF_RenderText_Solid(font,mot.c_str(),textColor);
  applySurface(x,y,mes,screen,NULL);
  SDL_FreeSurface(mes);
}
struct canard
{
    int couleur;  //0 noir 1 rouge et 2 bleu
    int vitesse;  //vitesse du canard (pour gérer son déplacement)
    int x;       //position x sur le champ de tir
    int y;       //position y sur le champ de tir
    int mvt;
};

struct niveau
{

};

int main ( int argc, char** argv )
{
    bool quit=false;
    SDL_Surface *screen, *ecran, *background;
    SDL_Event event;
    TTF_Init();
    TTF_Font *fonts;
    int fontSize=50;
    string police="duck_hunt.ttf";
    SDL_Color textColor={255,255,255};
    fonts = TTF_OpenFont(police.c_str(),fontSize);
    SDL_Init(SDL_INIT_EVERYTHING);
    screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,
            SCREEN_BPP,SDL_SWSURFACE);
    ecran=load_image("menu.bmp");
    background=load_image("backGameBlit.bmp");

    int menu = 0;
    int select=0;
    while (!quit && menu==0)
    {
        SDL_FillRect(screen,&screen->clip_rect, SDL_MapRGB(screen->format,61, 43, 31));
        applySurface(0,0,ecran,screen,NULL);


        ostringstream mssg;
        ostringstream mssg1;
        ostringstream mssg2;
        ostringstream mssg3;

        mssg.flush();
        mssg.str("mode classique");
        mssg1.flush();
        mssg1.str("quit");
        mssg2.flush();
        mssg2.str("DUCK HUNT");
        mssg3.flush();
        mssg3.str("-->");

        showMessageScreen(mssg.str(),190,250,fonts,fontSize,textColor,screen);
        showMessageScreen(mssg1.str(),320,310,fonts,fontSize,textColor,screen);
        showMessageScreen(mssg2.str(),10,10,fonts,fontSize,textColor,screen);

        while(SDL_PollEvent(&event))
            if(event.type==SDL_QUIT)
                quit=true;

        Uint8 *keystates = SDL_GetKeyState( NULL );

        if (keystates[ SDLK_UP ])
        {

            showMessageScreen(mssg3.str(),130,250,fonts,fontSize,textColor,screen);
            select=1;

        }
        if (keystates[ SDLK_DOWN ])
        {

            showMessageScreen(mssg3.str(),250,310,fonts,fontSize,textColor,screen);
            select=2;
        }

        else if(event.type == SDL_KEYUP )
        {
          //associons pour chaque flèche directionnelle un déplacement plus important
           switch( event.key.keysym.sym )
           {
              case SDLK_UP: showMessageScreen(mssg3.str(),130,250,fonts,fontSize,textColor,screen); break;
              case SDLK_DOWN: showMessageScreen(mssg3.str(),250,310,fonts,fontSize,textColor,screen); break;
           }
        }
        if ((keystates[ SDLK_RETURN]) && select==1)
        {
            menu=1;
        }
        if ((keystates[ SDLK_RETURN]) && select==2)
        {
            quit=true;
        }

        SDL_FreeSurface(screen);
        SDL_Flip(screen);
        }

        while (!quit && menu==1)
        {
            SDL_FillRect(screen,&screen->clip_rect, SDL_MapRGB(screen->format,61, 43, 31));
            applySurface(0,0,background,screen,NULL);
            SDL_FreeSurface(screen);
        SDL_Flip(screen);
        }

        SDL_FreeSurface(screen);
        SDL_Flip(screen);
    }

