#ifndef INIT_H
#define INIT_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <fmodex/fmod.h>

void initSDL(SDL_Surface **screen);
void initFMOD(FMOD_SYSTEM **sys);
void cleanUp(FMOD_SYSTEM **sys, SDL_Surface **a, SDL_Surface **b);

#endif
