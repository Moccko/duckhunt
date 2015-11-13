#ifndef DUCKHUNT_H_INCLUDED
#define DUCKHUNT_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <sstream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int SCREEN_BPP=32;
const int NB_CANARD_TO_WIN = 6;
const int NB_CANARD_PAR_NIVEAU = 10;

struct Canard
{
  int c;       //0 noir 1 rouge et 2 bleu
  int v;       //vitesse du canard (pour gérer son déplacement)
  int x;       //position x sur le champ de tir
  int y;       //position y sur le champ de tir
  int w;       // largeur du canard
  int h;       // longueur du canard
  int mvt;     // mouvement du canard
  //... et encore plusieurs autres....
};

struct Souris
{
    int x;
    int y;
};

SDL_Surface *load_image(std::string filename);
SDL_Surface * loadImageWithColorKey(std::string filename, int r, int g, int b);
void applySurface(int x, int y, SDL_Surface* source,
                                SDL_Surface* destination, SDL_Rect* clip);
bool collision(SDL_Surface* Screen, Canard D);

#endif // DUCKHUNT_H_INCLUDED
