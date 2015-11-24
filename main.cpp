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

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int SCREEN_BPP=32;
const int NB_CANARD_TO_WIN = 6;
const int NB_CANARD_PAR_NIVEAU = 10;
const int tailleRet=50;

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

void showMessageScreen(string message,int x,int y, TTF_Font *font,int fontsize,SDL_Color textColor,SDL_Surface* &screen)
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
        x+=fontsize;
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
    int c;  //0 noir 1 rouge et 2 bleu
    int v;  //vitesse du canard (pour gérer son déplacement)
    int x;       //position x sur le champ de tir
    int y;       //position y sur le champ de tir
    int w;
    int h;
    int mvt_x;
    int mvt_y;
    SDL_Surface *source;
    SDL_Rect lecture;
    SDL_Rect lectureFront;
};

struct Souris
{
    int x;
    int y;

    /*SDL_Surface *source;
    SDL_Rect lecture;
    SDL_Rect lectureFront;*/
};

struct niveau
{

};

void initCanard(canard &c)
{
    c.x=rand()%SCREEN_WIDTH-50;
    c.y=rand()%SCREEN_HEIGHT-100;
    c.h=49;
    c.w=53;
    c.c=rand()%3;
    // c.lectureFront.x=0;
    //  c.lectureFront.y=0;
    //   c.lectureFront.w=49;
    // c.lectureFront.h=53;
    //   c.lecture = c.lectureFront;
    if (c.c==0)
    {
        c.lectureFront.x=0;
        c.lectureFront.y=0;
        c.lectureFront.w=38;
        c.lectureFront.h=38;
        c.lecture = c.lectureFront;
        c.mvt_x=1;
        c.mvt_y=1;
        c.source=loadImageWithColorKey("k1.bmp", 144, 208, 136);
    }
    else if (c.c==1)
    {
        c.lectureFront.x=0;
        c.lectureFront.y=0;
        c.lectureFront.w=47;
        c.lectureFront.h=63;
        c.lecture = c.lectureFront;
        c.mvt_x=2;
        c.mvt_y=2;
        c.source=loadImageWithColorKey("k2.bmp", 144, 208, 136);
    }
    else
    {
        c.lectureFront.x=0;
        c.lectureFront.y=0;
        c.lectureFront.w=49;
        c.lectureFront.h=53;
        c.lecture = c.lectureFront;
        c.mvt_x=3;
        c.mvt_y=3;
        c.source=loadImageWithColorKey("k3.bmp", 144, 208, 136);
    }
}

/*void initReticule(Souris &r)
{
    r.x=SCREEN_WIDTH/2;
    r.y=SCREEN_HEIGHT/2;
}*/

void showReticule(Souris &r, SDL_Surface *pointeur, SDL_Surface *screen)
{
    applySurface(r.x-tailleRet/2,r.y-tailleRet/2,pointeur,screen,NULL);
}

int SDL_ShowCursor(int toggle);

void showMenu(int menu, string choice1, string choice2, TTF_Font *font, SDL_Surface *background, SDL_Surface* screen, SDL_Event event)
{
    applySurface(0,0,background,screen,NULL);
    showMessageScreen(choice1,SCREEN_WIDTH/2,SCREEN_HEIGHT/2-50,font,36,{255,255,255},screen);
    showMessageScreen(choice2,SCREEN_WIDTH/2,SCREEN_HEIGHT/2+50,font,36,{255,255,255},screen);
}

int main ( int argc, char** argv )
{
    bool quit=false;
    SDL_Surface *screen, *ecranMenu, *fondEcran, *reticule;
    SDL_Event event;
    TTF_Init();
    TTF_Font *font;
    int fontsize=50;
    string police="duck_hunt.ttf";
    SDL_Color textColor={255,255,255};
    font = TTF_OpenFont(police.c_str(),fontsize);
    SDL_Init(SDL_INIT_EVERYTHING);
    screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,
                            SCREEN_BPP,SDL_SWSURFACE);
    ecranMenu=load_image("menu1.bmp");
    fondEcran=load_image("back.bmp");
    reticule=loadImageWithColorKey("reticule.bmp",255,255,255);

    srand(time(NULL));
    canard j1;
    Souris souris;
    initCanard(j1);
    initCanard(j1);
    //initReticule(souris);
    int menu = 0;
    int select=0;
    while (!quit && menu==0)
    {
        SDL_FillRect(screen,&screen->clip_rect, SDL_MapRGB(screen->format,61, 43, 31));
        applySurface(0,0,ecranMenu,screen,NULL);

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
        mssg3.str("->");

        //showMenu(menu,mssg.str(),mssg2.str(),font,ecranMenu,screen,event);

        showMessageScreen(mssg.str(),190,250,font,fontsize,textColor,screen);
        showMessageScreen(mssg1.str(),320,310,font,fontsize,textColor,screen);
        showMessageScreen(mssg2.str(),10,10,font,fontsize,textColor,screen);

        while(SDL_PollEvent(&event))
            if(event.type==SDL_QUIT)
                quit=true;

        Uint8 *keystates = SDL_GetKeyState( NULL );

        if (keystates[ SDLK_UP ])
        {

            showMessageScreen(mssg3.str(),130,250,font,fontsize,textColor,screen);
            select=1;

        }
        if (keystates[ SDLK_DOWN ])
        {

            showMessageScreen(mssg3.str(),250,310,font,fontsize,textColor,screen);
            select=2;
        }

        else if(event.type == SDL_KEYUP )
        {
            switch( event.key.keysym.sym )
            {
            case SDLK_UP: showMessageScreen(mssg3.str(),130,250,font,fontsize,textColor,screen); break;
            case SDLK_DOWN: showMessageScreen(mssg3.str(),250,310,font,fontsize,textColor,screen); break;
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
        applySurface(0,0,fondEcran,screen,NULL);
        applySurface(j1.x,j1.y,j1.source,screen, &j1.lecture);

        j1.x+=j1.mvt_x;
        j1.y+=j1.mvt_y;
        if(j1.y+j1.h>SCREEN_HEIGHT-170)
        {
            j1.y=SCREEN_HEIGHT-170-j1.h;
            j1.mvt_y*=-1;
        }
        if(j1.y<0)
        {
            j1.y=0;
            j1.mvt_y*=-1;
        }
        if(j1.x+j1.w>SCREEN_WIDTH)
        {
            j1.x=SCREEN_WIDTH-j1.w;
            j1.mvt_x*=-1;
        }
        if(j1.x<0)
        {
            j1.x=0;
            j1.mvt_x*=-1;
        }

        SDL_ShowCursor(0);

        while(SDL_PollEvent(&event))
            if(event.type==SDL_MOUSEMOTION)
                souris.x=event.motion.x;
        souris.y=event.motion.y;

        showReticule(souris,reticule,screen);



        SDL_Flip(screen);
        SDL_FreeSurface(screen);
        SDL_Flip(screen);
    }

    SDL_FreeSurface(screen);
    SDL_Flip(screen);
    SDL_Quit();
}
