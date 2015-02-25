#ifndef cTc_H
#define cTc_H
const MAP_SIZE = 8 ;

typedef struct 
{
    int x ;
    int y ;
} Coordinates ;

typedef struct
{
    Coordinates position ;
    Cercle *cercle;
    Cat *cat;/////////////////////////////////////////////////////////////
} Object ;

typedef struct
{
   Object ** mat[MAP_SIZE][MAP_SIZE] ;
} Map ;

typedef struct
{
    int etat ;
    size_t rayon ;
    int priorite ;
    Color couleur ;
} Cercle ;

typedef struct
{
    Coordinates position ;
} Chat ;

#endif
