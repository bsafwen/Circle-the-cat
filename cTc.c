#include "cTc.h"

void initializeMap(Map *m, Coordinates *c)
{
    int i ;
    int j ;
    for(i=0;i<MAP_SIZE;++i)
        for(j=0;j<MAP_SIZE;++j)
        {
               m->mat[i][j]=malloc(sizeof(Object));
               m->mat[i][j]->etat='O';
               m->mat[i][j]->cat=0;
               if( i <= (MAP_SIZE / 2 ) )
               {
                 if( j < i )
                {
                       m->mat[i][j]->priorite = MAP_SIZE/2 + 1  - j ;
                }
                else if(j > MAP_SIZE - 1 - i)
                {
                    m->mat[i][j]->priorite = MAP_SIZE/2 + 1  - j % (MAP_SIZE/2)  ;
                }
                else
                {
                    m->mat[i][j]->priorite = MAP_SIZE/2 + 1  - i  ;
                }
               }
               else
               {
                    m->mat[i][j]->priorite = m->mat[MAP_SIZE - 1 - i][j]->priorite;
               }
        }
    m->mat[c->x][c->y]->etat='C';
}

int existWay(Coordinates *ch, Map *m) // 1 => there is a way out
{
    int i,j;
    for(i=ch->x-1;i<=ch->x+1;++i){
        for(j=ch->y-1;j<=ch->y+1;j++){
            if (!(i < 0 || j < 0 || i >= MAP_SIZE || j >= MAP_SIZE )){
               if (m->mat[i][j]->etat=='O')
                    return 1 ;
                }
             }
    }
    return 0 ;
}

void chaseCat(Map *m)
{
    Coordinates coo;
    printf("Donnez les coordonneés de la case à occuper : ");
    scanf("%d %d",&coo.x,&coo.y);
    if(m->mat[coo.x][coo.y]->etat != 'X')
       m->mat[coo.x][coo.y]->etat='X';
    else
        printf("Erreur : position invalide.\n");
}

void moveCat(Map *m, Coordinates *c)
{
    Coordinates max;
    int mx=-1, i , j;
    for(i=c->x-1;i<=c->x+1;++i){
        for(j=c->y-1;j<=c->y+1;j++){
           if(m->mat[i][j]->priorite>mx && m->mat[i][j]->etat != 'X')
           {
               max.x=i;
               max.y=j;
               j=2+c->y;
               i=2+c->x;
           }
        }
    }
    m->mat[c->x][c->y]->etat='O';
    m->mat[max.x][max.y]->etat='C' ;
    c->x = max.x;
    c->y = max.y;
}

int gameOver(Map *m,Coordinates *ch) // return 1 if the cat has escaped.\n
{
    if(ch->x==0 || ch->y==0 || ch->x==MAP_SIZE-1 || ch->y==MAP_SIZE-1 || (existWay(ch,m)==0))
    {
        return 1 ;
    }
    return 0 ;
}

void quitGame(Map *m)
{
    int i,j;
    for ( i = 0 ; i < MAP_SIZE ; ++i )
        for ( j = 0 ; j < MAP_SIZE ; ++j )
            free(m->mat[i][j]);
}

void draw(Map *m)
{
    
     int i,j,k;
     printf("\n");
     for ( i = 0 ; i < 4 * MAP_SIZE ; ++i )
     {
         printf("~");
     }
     printf("\n");
     for ( i = 0 ; i < MAP_SIZE ; ++i)
     {
         printf("| ");
         for ( j = 0 ; j < MAP_SIZE ; ++j)
         {
             printf("%c | ",m->mat[i][j]->etat);
         }
     for ( k = 0 ; k < 4 * MAP_SIZE ; ++k )
     {
         printf("~");
     }
     }
  
}


