#include "duckhunt.h"

using namespace std;

/****************** Nom de la fonction **********************
* load_image                                                *
********************* Description ***************************
* Charger une image                                         *
*********************** Entrées *****************************
* Nom du fichier de l'image                                 *
*********************** Sorties *****************************
* Renvoie une image chargée                                 *
************************************************************/

SDL_Surface *load_image(std::string filename)
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = SDL_LoadBMP( filename.c_str() );
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
    }
    return optimizedImage;
}

/****************** Nom de la fonction **********************
* loadImageWithColorKey                                     *
********************* Description ***************************
* Charger une image en prenant en charger la transparence   *
*********************** Entrées *****************************
* Nom du fichier de l'image                                 *
*********************** Sorties *****************************
* Renvoie une image chargée                                 *
************************************************************/

SDL_Surface * loadImageWithColorKey(string filename, int r, int g, int b)
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
        if( optimizedImage != NULL )
        {
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, r, g, b );
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
    }
    return optimizedImage;
}

void applySurface(int x, int y, SDL_Surface* source,
                  SDL_Surface* destination, SDL_Rect* clip)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset );
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


    struct joueur
    {
        int x;
        int y;
        int w;
        int h;
        SDL_Surface *source;
        SDL_Rect lecture;
        SDL_Rect lectureFront;
        SDL_Rect lectureBack;

    };

    struct ball
    {
        int x;
        int y;
        int w;
        int h;
        SDL_Surface *source;
        SDL_Rect lecture;
        SDL_Rect lectureFront;
    };

    void initjoueur(joueur &s)
    {
        s.x=10;
        s.y=SCREEN_HEIGHT/2;
        s.w=15;
        s.h=59;
        s.lectureFront.x=0;
        s.lectureFront.y=0;
        s.lectureFront.w=15;
        s.lectureFront.h=59;
        s.lecture = s.lectureFront;
    }

    void initball(ball &b)
    {
        b.x=SCREEN_WIDTH/2;
        b.y=SCREEN_HEIGHT/2;
        b.h=16;
        b.w=16;
        b.lectureFront.x=0;
        b.lectureFront.y=0;
        b.lectureFront.w=16;
        b.lectureFront.h=16;
        b.lecture = b.lectureFront;
    }


    bool collision(SDL_Surface* Screen, Canard D)
    {
        int leftD;
        int rightD;
        int topD;
        int bottomD;

        leftS = s.x;
        rightS = s.x + s.w;
        topS = s.y;
        bottomS = s.y + s.h;

        leftD = d.x;
        rightD = d.x + d.w;
        topD = d.y;
        bottomD = d.y + d.h;

        if(bottomS >= topD)
            return false;
        if(topS <= bottomD)
            return false;
        if(rightS >= leftD)
            return false;
        if(leftS <= rightD)
            return false;

        return true;
    }
