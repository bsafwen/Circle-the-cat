#ifndef cTc_H
#define cTc_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_SIZE 11

typedef struct 
{
    int x ;
    int y ;
} Coordinates ;

typedef struct
{
      Coordinates *cat;
      char etat ;
      int priorite ;
} Object ;

typedef struct
{
   Object *mat[MAP_SIZE][MAP_SIZE] ;
} Map ;

void moveCat(Map *m, Coordinates *c);

int gameOver(Map *m, Coordinates *c);

void chaseCat(Map *m);

int existWay(Coordinates *c, Map *m);

void initializeMap(Map *m, Coordinates *c);

void draw(Map *m);

void quitGame(Map *m);
#endif
