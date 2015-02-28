#include <stdio.h>
#include <stdlib.h>
#include "cTc.h"

const int HIGH=10;
const int RAYON=5;
/*Object createObject(coordinates p, Color couleur)
{
    Object o ;
    o.position.x=p.x;
    o.position.y=p.y;
    o.cat=NULL;
    Cercle c;
    o.cercle=malloc(sizeof(Cercle));
    o.cercle->etat=0;
    o.cercle->rayon=RAYON;
    o.cercle->priorite=HIGH;
    o.cercle->couleur=Yellow;
    return o ;
}*/

void initializeMap(Map m)
{
    int i ;
    int j ;
    for(i=0;i<MAP_SIZE;++i)
        for(j=0;j<MAP_SIZE;++j)
        {
               m[i][j]=malloc(sizeof(Object));
               m[i][j]->position.x=p.x;
               m[i][j]->position.y=p.y;
               m[i][j]->cat=NULL;
               m[i][j]->cercle=malloc(sizeof(Cercle));
               m[i][j]->cercle->etat=0;
               m[i][j]->cercle->rayon=RAYON;
               //e.g:MAP_SIZE=7 => 3-2-1-0-1-2-3 les priorite augmente en allant vers les bords
               if(MAP_SIZE/2 - j >=0)
                     m[i][j]->cercle->priorite=MAP_SIZE/2-j;
               else
                     m[i][j]->cercle->priorite=j-MAP_SIZE/2;
        }
}

void changeColor(Coordinates coo, Color nC, Map m)
{
    m[coo.x][coo.y].cercle->couleur=nC;
}

void moveCat(Chat ch,Coordinates coo, Map m)
{
    m[ch.position.x][ch.position.y]->cat=NULL; 
    free(m[ch.position.x][ch.position.y]->cat);
    m[coo.x][coo.y]->cat=malloc(sizeof(Cat));
    m[coo.x][coo.y]->cat->x=coo.x;
    m[coo.x][coo.y]->cat->y=coo.y;
}

Cat createCat()
{
    Cat temp ;
    temp.position.x=0;
    temp.position.y=0;
    return temp ;
}

int existWay(Cat ch, Map m)
{
    int i,j;
    for(i=ch.position.x-1;i<=ch.position.x-1;++i){
        for(j=ch.position.y-1;j<=ch.position.y+1;j++){
            if !(i < 0 || j < 0 || i >= MAP_SIZE || j >= MAP_SIZE ){
               if (m[i][j].cercle->etat=1)
                    return 1 ;
                }
             }
    }
    return 0 ;
}

void chaseCat(Coordinates coo, Map m, Chat ch)
{
    free(m[coo.x][coo.y]->cat);
    m[coo.x][coo.y]->cat=malloc(sizeof(Chat));
    m[coo.x][coo.y]->cat->x=coo.x;
    m[coo.x][coo.y]->cat->y=coo.y;
}

Coordinates pathFinder(Map m, Chat c)
{
    Coordinates max;
    int mx=-1;
    for(i=ch.position.x-1;i<=ch.position.x-1;++i){
        for(j=ch.position.y-1;j<=ch.position.y+1;j++){
           if(m[i][j]->cercle->priorite>mx)
           {
               mx=m[i][j]->cercle->priorite;
               max.x=i;
               max.y=j;
           }
        }
    }
    return max ;
}
