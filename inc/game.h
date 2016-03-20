#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <fmodex/fmod.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL_ttf.h>

#define MAP_SIZE 11
#define CIRCLE_WIDTH 40
#define CIRCLE_HEIGHT 40
#define CAT_HEIGHT 22 
#define CAT_WIDTH  28 ;

enum { EAST, WEST, N_EAST, N_WEST, S_EAST, S_WEST };
enum { EASY, NORMAL, HARD } ;

typedef struct
{
    int etat ;
    int priorite ;
} objet;

typedef struct 
{
    int x ;
    int y ;
    int h ;
} Rect ;

enum { YELLOW, GREEN };

void draw(SDL_Surface **s);
void animCat(SDL_Surface **sPtr);
int  gameRun(SDL_Surface **screenPtr, FMOD_SYSTEM **sysPtr, int lvl, int *run);
int gameOver(void);
void chaseCat( int x, int y, int lvl);
void moveCat(int lvl);
Rect distanceMinimale(Rect t[6],int n);
int min(int a, int b, int c, int d);
void moveCatHard(void);
void moveCatNormal(void);
void moveCatEasy(void);
void initializeMap(void);
void initGame(SDL_Surface **sPtr, int lvl);
int howTo(SDL_Surface **s);
void scorE( int a, int b);

#endif
