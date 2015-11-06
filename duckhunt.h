#ifndef DUCKHUNT_H_INCLUDED
#define DUCKHUNT_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <sstream>

struct Canard
{
  int couleur;   //0 noir 1 rouge et 2 bleu
  int vitesse;    //vitesse du canard (pour gérer son déplacement)
  int posX;       //position x sur le champ de tir
  int posY;       //position y sur le champ de tir
  ... et encore plusieurs autres....
};

SDL_Surface *load_image(std::string filename);
SDL_Surface * loadImageWithColorKey(string filename, int r, int g, int b);
void applySurface(int x, int y, SDL_Surface* source,
                                SDL_Surface* destination, SDL_Rect* clip);
bool collision(SDL_Surface* Screen, Canard D);

#endif // DUCKHUNT_H_INCLUDED
