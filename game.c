#include "game.h"
#include "linkedList.h"
#include "dji.h"

const SDL_Color RED = { 255, 0, 0 } ;
const SDL_Color BLUE = { 0, 0, 255 } ;
const SDL_Color BLACK = { 0, 0, 0 } ;

#define SOMMET 121
#define IN 10000

Rect path[121];
int a = 0, cost[121][121]={{IN}} ;
FMOD_SOUND *jump = NULL, *music = NULL, *click = NULL, *laugh_cat = NULL, *angry_cat = NULL  ;
SDL_Rect oldCatPos,catPos, p ,p1, p2, p3, p4, p5, p6 ;
SDL_Surface *Pause = NULL, *reset = NULL, *score = NULL, *tryAgain = NULL, *youLose =NULL, *CLICK = NULL, *youWin = NULL, *yellow = NULL, *green = NULL, *cat = NULL, *ok_button = NULL, *ko_button = NULL, *temp = NULL ;
int continuer = 1, i = 0, j = 0, counter = 0, x = 0, y = 0, validClick = 0, nbClick = 0, actuel = 20, precedent = 0 ;
char nc[5] = {'0'};
objet  map[MAP_SIZE][MAP_SIZE] ;
TTF_Font *police = NULL, *policeNumber = NULL ;
SDL_Rect sprites[6][4];
int direction, paused = 0 ;
SDL_Rect lastPos ;

void scorE(int Level,int s)
{
    FILE* fichier = NULL;
    int i, SCORE[3];
    if(Level==1)
        fichier = fopen("highscore/Easy.txt", "r+");
    else if(Level==2)
        fichier = fopen("highscore/Medium.txt","r+");
    else if(Level==3)
        fichier = fopen("highscore/Hard.txt", "r+");
    else 
        printf("error : invalid level.\n");
    if (fichier != NULL)
    {
        fscanf(fichier, "%d %d %d", &SCORE[0], &SCORE[1], &SCORE[2]);
        if (s<=SCORE[0])
        {SCORE[2]=SCORE[1];
            SCORE[1]=SCORE[0];
            SCORE[0]=s;}
        if(s<=SCORE[1]&&s>SCORE[0])
        {SCORE[2]=SCORE[1];
            SCORE[1]=s;}
        if(s<SCORE[2]&&s>SCORE[1])
        {SCORE[2]=s;}

        fclose(fichier);

        if(Level==1)
            fichier = fopen("highscore/Easy.txt", "w+");
        else if(Level==2)
            fichier = fopen("highscore/Medium.txt","w+");
        else if(Level==3)
            fichier = fopen("highscore/Hard.txt", "w+");
        else 
            printf("error : invalid level.\n");

        if (fichier != NULL)
        {

            for(i=0;i<3;i++)
            {
                fprintf(fichier, "%d\n", SCORE[i]);
            }


        }
        fclose(fichier);
    }
    else 
    {
        printf("Error : Could not open file.\n");
    }
}

void initCost(void)
{
    for ( i = 0 ; i < SOMMET ; ++i )
    {
        for ( j = 0 ; j < SOMMET ; ++j )
        {
            if ( ((i/MAP_SIZE)+1)%2!=0)
            {
                if ( (j == i-1 && (i%MAP_SIZE)-1>=0) || (j == i+1 && (i%MAP_SIZE)+1<MAP_SIZE) || (j == i - MAP_SIZE) || (j == i - MAP_SIZE -1 && ((i-MAP_SIZE)%MAP_SIZE-1>=0)) || j == i + MAP_SIZE ||( j == i + MAP_SIZE -1 &&((i-MAP_SIZE)%MAP_SIZE-1>=0)))
                    cost[i][j] = 10 ;
                else cost[i][j] = IN ;
            }
            else
            {
                if ( (j == i-1 && (i%MAP_SIZE)-1>=0) || (j == i+1 && (i%MAP_SIZE)+1<MAP_SIZE) || (j == i - MAP_SIZE) || (j == i - MAP_SIZE +1 && ((i-MAP_SIZE)%MAP_SIZE+1<MAP_SIZE)) || j == i + MAP_SIZE ||( j == i + MAP_SIZE +1 &&((i-MAP_SIZE)%MAP_SIZE+1<MAP_SIZE)))
                    cost[i][j] = 10 ;
                else cost[i][j] = IN ;
            }
        }
        cost[i][i] = IN ;
    }
}
void chaseCat( int x, int y, int level )
{
    int i , j ;
    map[x][y].etat = GREEN ;
    if ( level == HARD )
        for ( j = 0 ; j <  SOMMET ; ++j )
            cost[j][x*MAP_SIZE +y] = IN ;
    for(i=x-1;i<=x+1;++i)
    { 
        for(j=y-1;j<=y+1;j++){
            if ( (x+1) % 2 != 0 )
            {
                if (( i == x - 1 && j == y - 1 ) || ( i == x + 1 && j == y - 1))
                    continue ;
            }
            else
            {
                if (( i == x - 1 && j == y + 1 ) || ( i == x + 1 && j == y + 1))
                    continue ;
            }
            if (!(i < 0 || j < 0 || i >= MAP_SIZE || j >= MAP_SIZE ))
            {
                map[i][j].priorite--;
            }
        }
    }
}

int min(int a, int b, int c, int d)
{   
    int t[4];
    t[0]=a;
    t[1]=b;
    t[2]=c;
    t[3]=d; 
    int min = MAP_SIZE * 2 ;
    int i,index;
    for ( i = 0 ; i < 4 ; ++i )
    {   
        if(t[i] < min)
        {   
            min = t[i] ;
            index = i ;
        }
    }
    return t[index];
}

Rect distanceMinimale(Rect t[], int n)
{   
    Rect temp ; 
    temp.x = temp.y = -1 ;
    int m = 200 ;
    int i ;
    int distance ;
    for ( i = 0 ; i < n ; ++i )
    {   
        if(t[i].x>=0)
        {
            distance = min(t[i].x,t[i].y,MAP_SIZE-1-t[i].x,MAP_SIZE-1-t[i].y);
            if(distance < m)
            {
                m = distance ;
                temp = t[i];
            }
        }
    }
    return temp ;
}

void moveCat(int level)
{
    if ( level == EASY )
        moveCatEasy();
    else if ( level == NORMAL )
        moveCatNormal();
    else if ( level == HARD )
        moveCatHard();
}

void moveCatHard(void)
{
    int i,j;
    Rect temp ;
    list wayOut = NULL ;
    int index = 1 ;
    for ( i = 0 ; i <SOMMET ; ++i) cost[i][catPos.x*MAP_SIZE+catPos.y]+=3;
    for ( i = 0 ; i < 11 ; ++i )
    {
        if ( map[0][i].etat != GREEN )
            insert(&wayOut,i,index++);
        if ( map[10][i].etat != GREEN )
            insert(&wayOut,10*MAP_SIZE+i,index++);
        if ( i != 0 && i != 10 )
        {
            if ( map[i][0].etat != GREEN )
                insert(&wayOut, i * MAP_SIZE, index++ ) ;
            if ( map[i][10].etat != GREEN )
                insert(&wayOut,i*MAP_SIZE + 10,index++);
        }
    }
    i=j=0;
    int cP ;
    cP = MAP_SIZE * catPos.x + catPos.y ;
    while ( wayOut != NULL )
    {
        path[i++]=dji(cost,cP,wayOut->value);
        wayOut=wayOut->nxt;
    }
    temp = miin(path,i);
    if ( temp.h >= IN )
    {
        for(i=catPos.x-1;i<=catPos.x+1;++i){
            for(j=catPos.y-1;j<=catPos.y+1;j++){
                if ( (catPos.x == i) && (catPos.y == j) )
                    continue;
                if ( (catPos.x+1) % 2 != 0 )
                {
                    if (( i == catPos.x + 1 && j == catPos.y + 1 ) || ( i == catPos.x - 1 && j == catPos.y + 1))
                        continue ;
                }
                else
                {
                    if (( i == catPos.x + 1 && j == catPos.y - 1 ) || ( i == catPos.x - 1 && j == catPos.y - 1))
                        continue ;
                }
                if ( ! ( i < 0 || j < 0 || i >= MAP_SIZE || j >= MAP_SIZE) )
                {
                    if(map[i][j].etat != GREEN)
                    {
                        catPos.x = i ;
                        catPos.y = j ;
                        i=j=IN;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        catPos.x = temp.x ;
        catPos.y = temp.y ;
    }
    clean(&wayOut);
}



void moveCatNormal(void)
{
    Rect max , tab[6];
    int mx=-100, i , j, k=0 ;
    max.x = max.y = -1 ;
    for( i = 0 ; i < 6 ; ++i )
        tab[i].x=tab[i].y=tab[i].h=-5;

    for(i=catPos.x-1;i<=catPos.x+1;++i){
        for(j=catPos.y-1;j<=catPos.y+1;j++){
            if ( (catPos.x == i) && (catPos.y == j) )
                continue;
            if ( (catPos.x+1) % 2 != 0 )
            {
                if (( i == catPos.x + 1 && j == catPos.y + 1 ) || ( i == catPos.x - 1 && j == catPos.y + 1))
                    continue ;
            }
            else
            {
                if (( i == catPos.x + 1 && j == catPos.y - 1 ) || ( i == catPos.x - 1 && j == catPos.y - 1))
                    continue ;
            }
            if ( ! ( i < 0 || j < 0 || i >= MAP_SIZE || j >= MAP_SIZE) )
            {
                if((i==0 || j==0 || i==MAP_SIZE-1 || j==MAP_SIZE-1) && map[i][j].etat != GREEN )
                {
                    tab[k].x = i ;
                    tab[k].y = j ;
                    mx = tab[k].h = map[i][j].priorite;
                    ++k;
                    i = j = 1000 ;
                    break ;
                }
                if(map[i][j].etat != GREEN)
                {
                    tab[k].x=i;
                    tab[k].y=j;
                    tab[k].h=map[i][j].priorite;
                    ++k;

                    if( map[i][j].priorite > mx )
                    {
                        mx=map[i][j].priorite;
                    }
                }
            }
        }
    }
    for ( i = 0 ; i < k ; ++i )
    {
        if ( tab[i].h != mx )
            tab[i].x = -5 ;
    }
    max = distanceMinimale(tab,k) ;
    map[catPos.x][catPos.y].priorite-=2 ;
    catPos.x = max.x;
    catPos.y = max.y;
}

void moveCatEasy(void)
{
    Rect max , tab[6];
    int  i , j, k=0 ;
    max.x = max.y = -1 ;

    for(i=catPos.x-1;i<=catPos.x+1;++i){
        for(j=catPos.y-1;j<=catPos.y+1;j++){
            if ( (catPos.x == i) && (catPos.y == j) )
                continue;
            if ( (catPos.x+1) % 2 != 0 )
            {
                if (( i == catPos.x + 1 && j == catPos.y + 1 ) || ( i == catPos.x - 1 && j == catPos.y + 1))
                    continue ;
            }
            else
            {
                if (( i == catPos.x + 1 && j == catPos.y - 1 ) || ( i == catPos.x - 1 && j == catPos.y - 1))
                    continue ;
            }
            if ( ! ( i < 0 || j < 0 || i >= MAP_SIZE || j >= MAP_SIZE) )
            {
                if(map[i][j].etat != GREEN)
                {
                    tab[k].x=i;
                    tab[k].y=j;
                    tab[k].h=map[i][j].priorite;
                    ++k;
                }
            }
        }
    }

    max = tab[rand()%k] ;
    map[catPos.x][catPos.y].priorite-=2 ;
    catPos.x = max.x;
    catPos.y = max.y;
}

void draw(SDL_Surface **sPtr)
{
    SDL_Rect p ;
    int i = 0 , j = 0 ;
    SDL_FillRect( *sPtr, 0, SDL_MapRGB((*sPtr)->format, 255, 255, 255));
    for ( i = 0 ; i < 11 ; ++i )
    {
        if ( ( i +  1 ) % 2 == 0 )
            p.x = CIRCLE_HEIGHT / 2 ;
        else
            p.x = 0 ;
        p.y = CIRCLE_WIDTH * i ;
        for ( j = 0 ; j < 11 ; ++j )
        {
            if(map[i][j].etat == YELLOW)
                SDL_BlitSurface(yellow, NULL, *sPtr, &p);
            else if ( map[i][j].etat == GREEN )
                SDL_BlitSurface(green, NULL, *sPtr, &p);
            p.x += CIRCLE_HEIGHT  ;
        }
    }
    SDL_Flip(*sPtr);
}

int existWay(void)
{
    int i,j;
    for(i=catPos.x-1;i<=catPos.x+1;++i){
        for(j=catPos.y-1;j<=catPos.y+1;j++){
            if ( (catPos.x+1) % 2 != 0 )
            {   
                if (( i == catPos.x + 1 && j == catPos.y + 1 ) || ( i == catPos.x - 1 && j == catPos.y + 1))
                    continue ;
            }
            else
            {   
                if (( i == catPos.x + 1 && j == catPos.y - 1 ) || ( i == catPos.x - 1 && j == catPos.y - 1))
                    continue ;
            }
            if ( i == catPos.x && j == catPos.y )
                continue ;
            if (!(i < 0 || j < 0 || i >= MAP_SIZE || j >= MAP_SIZE )){
                if (map[i][j].etat==YELLOW)
                    return 1 ;
            }
        }
    }
    return 0 ;
}


int gameOver(void)
{
    if(catPos.x==0 || catPos.y==0 || catPos.x==MAP_SIZE-1 || catPos.y==MAP_SIZE-1 || (existWay()==0))
    {   
        return 1 ;
    }
    return 0 ;
}

void initializeMap(void)
{
    int i,j,x,y,counter;
    for(i=0;i<MAP_SIZE;++i)
        for(j=0;j<MAP_SIZE;++j)
        {
            map[i][j].etat=YELLOW;
            if( i <= (MAP_SIZE / 2 ) )
            {
                if( j < i )
                {
                    map[i][j].priorite = 2*( MAP_SIZE/2 + 1  - j );
                }
                else if(j > MAP_SIZE - 1 - i)
                {
                    map[i][j].priorite = 2*( MAP_SIZE/2 + 1  - (MAP_SIZE - 1 - j ) );
                }
                else
                {
                    map[i][j].priorite = 2*( MAP_SIZE/2 + 1  - i ) ;
                }
            }
            else
            {
                map[i][j].priorite = map[MAP_SIZE - 1 - i][j].priorite;
            }
        }
    srand( time(NULL) );
    counter = rand()%8 + 5 ;
    for ( i = 0 ; i < counter ; ++i )
    {
        x = rand() % MAP_SIZE ;
        y = rand() % MAP_SIZE ;
        if( x != 5 || y != 5 )
        {
            map[x][y].etat = GREEN ;
            --map[x][y].priorite ;
            for(j=0;j<SOMMET;++j)
                cost[j][x*MAP_SIZE+y]=IN;
        }
    }
    for ( i = 1 ; i <= 9 ; ++i )
    {
        if ( i % 2 == 0 )
            for ( j = 0 ; j < SOMMET ; ++j )
            {
                cost[j][i*MAP_SIZE+1]-=10 ;
            }
        else
            for ( j = 0 ; j < SOMMET ; ++j )
            {
                cost[j][i*MAP_SIZE+9]-=10 ;
            }
    }

}

void displayText(SDL_Surface **sPtr)
{
    sprintf(nc,"%d",nbClick);
    CLICK = TTF_RenderText_Blended(policeNumber, nc,BLUE);
    SDL_BlitSurface( score, NULL, *sPtr, &p5 );
    SDL_BlitSurface( CLICK, NULL, *sPtr, &p4 );
    SDL_BlitSurface( reset, NULL, *sPtr, &p6 );
    SDL_Flip(*sPtr);
}

void initGame(SDL_Surface **sPtr, int level)
{
    if( level == HARD )initCost();
    SDL_Rect convertedPos, pInit ;
    catPos.x = catPos.y = 5 ;
    oldCatPos = catPos ;
    convertedPos.x = 5 * 40 + 14 + 20 ;
    convertedPos.y = 5 * 40 + 11 ;
    pInit.y = 141 ;
    pInit.x = 137 ;
    pInit.h = 18 ;
    pInit.w = 13 ;
    nbClick = 0 ;
    initializeMap() ;
    draw(sPtr);
    displayText(sPtr);
    SDL_BlitSurface( cat, &pInit, *sPtr, &convertedPos );
    SDL_Flip(*sPtr);
}

void playAnim(SDL_Surface **sPtr )
{
    int i, pasX=0, pasY=0;
    SDL_Rect pBlit ;
    if( ( oldCatPos.x + 1) % 2 != 0 )
    {
        pBlit.x = ( oldCatPos.y * 40 ) +14  ;
        pBlit.y = ( oldCatPos.x * 40 ) + 11 ;
    }
    else
    {
        pBlit.x = oldCatPos.y * 40 + 20 + 14 ;
        pBlit.y = oldCatPos.x * 40 + 11 ;
    }
    if (( oldCatPos.x != catPos.x) || ( oldCatPos.y != catPos.y ))
    {
        if (( oldCatPos.x + 1 ) % 2 == 0 )
        {
            if ( (catPos.x - oldCatPos.x < 0) && (catPos.y - oldCatPos.y == 0 ) )
            {
                pasX = -7 ;
                pasY = -13 ;
                direction = N_WEST ;
                pBlit.x -= 4 ;
                pBlit.y -= 4 ;
            }
            if ( (catPos.x - oldCatPos.x < 0) && (catPos.y - oldCatPos.y > 0 ) )
            { 
                pasX = 5;
                pasY = -13;
                direction = N_EAST ;
                pBlit.y -= 4 ;
            }
            if ( (catPos.x - oldCatPos.x == 0) && (catPos.y - oldCatPos.y < 0 ) )
            {
                pasX = -13 ;
                direction = WEST ;
                pBlit.x -= 7 ;
            }
            if ( (catPos.x - oldCatPos.x == 0) && (catPos.y - oldCatPos.y > 0 ) )
            {
                pasX = +11 ;
                direction = EAST ;
            }
            if ( (catPos.x - oldCatPos.x > 0) && (catPos.y - oldCatPos.y == 0 ) )
            {
                pasX = -7 ;
                pasY = 14 ;
                pBlit.x -= 6 ;
                direction = S_WEST ;
            }
            if ( (catPos.x - oldCatPos.x > 0) && (catPos.y - oldCatPos.y > 0 ) )
            {
                pasX = 6 ;
                pasY = 14 ;
                direction = S_EAST ;
            }
        }
        else 
        {
            if ( (catPos.x - oldCatPos.x < 0) && (catPos.y - oldCatPos.y < 0 ) )
            {
                pasX = -7 ;
                pasY = -13 ;
                direction = N_WEST ;
                pBlit.x -= 4 ;
                pBlit.y -= 4 ;
            }
            if ( (catPos.x - oldCatPos.x < 0) && (catPos.y - oldCatPos.y == 0 ) )
            {
                pasX = 5;
                pasY = -13;
                direction = N_EAST ;
                pBlit.y -= 4 ;
            }
            if ( (catPos.x - oldCatPos.x == 0) && (catPos.y - oldCatPos.y < 0 ) )
            {
                pasX = -13 ;
                pBlit.x -= 7 ;
                direction = WEST ;
            }
            if ( (catPos.x - oldCatPos.x == 0) && (catPos.y - oldCatPos.y > 0 ) )
            {
                pasX = 11 ;
                direction = EAST ;
            }
            if ( (catPos.x - oldCatPos.x > 0) && (catPos.y - oldCatPos.y < 0 ) )
            {
                pasX = -7 ;
                pasY = 14 ;
                pBlit.x -= 6 ;
                direction = S_WEST ;
            }
            if ( (catPos.x - oldCatPos.x > 0) && (catPos.y - oldCatPos.y == 0 ) )
            {
                pasX = 6 ;
                pasY = 14 ;
                direction = S_EAST ;
            }
        }
        for ( i = 0 ; i < 4 ; ++i )
        {
            draw(sPtr);
            displayText(sPtr);
            SDL_BlitSurface( cat, &sprites[direction][i], *sPtr, &pBlit );
            SDL_Flip(*sPtr);
            SDL_Delay(90);
            pBlit.x += pasX ;
            pBlit.y += pasY ;

        }
        lastPos = pBlit ;
        lastPos.x -= pasX ;
        lastPos.y -= pasY ;
    }
    else if ( (oldCatPos.x == catPos.x) && (oldCatPos.y == catPos.y ) )
    {

        pBlit.x -= 7 ;
        draw(sPtr);
        displayText(sPtr);
        SDL_BlitSurface( cat, &sprites[direction][3], *sPtr, &pBlit );
        SDL_Flip(*sPtr);
    }

}

void unpause(SDL_Surface **screenPtr)
{
    SDL_BlitSurface( temp, NULL, *screenPtr,NULL );
    SDL_Flip(*screenPtr);
}

void setFrames()
{
    sprites[EAST][0].x = 5 ;
    int i,j ;
    for ( i =  0 ; i < 4 ; ++i )
    {
        sprites[EAST][i].y = 74 ;
    }
    for( i = 0 ; i < 6 ; ++i )
    {
        for( j = 0 ; j < 4 ; ++j )
        {

            sprites[i][j].h = CAT_HEIGHT ;
            sprites[i][j].w = CAT_WIDTH ;
        }
    }
    for ( i = 1 ; i < 3 ; ++i )
    {
        sprites[EAST][i].x = sprites[EAST][i-1].x + 31 ;
    }
    sprites[EAST][3].x = 134 ;
    sprites[EAST][3].y = 205 ;
    for ( i =  0 ; i < 4 ; ++i )
    {
        sprites[WEST][i].y = 42 ;
    }
    sprites[WEST][0].x = 66;
    sprites[WEST][1].x = 34;
    sprites[WEST][2].x = 2 ;
    sprites[WEST][3].x = 130 ;
    sprites[WEST][3].y = 173 ;
    for ( i =  0 ; i < 4 ; ++i )
    {
        sprites[N_EAST][i].y = 105 ;
    }
    sprites[N_EAST][0].x = 292;
    sprites[N_EAST][1].x = 326 ;
    sprites[N_EAST][2].x = 356 ;
    sprites[N_EAST][3].x = 166 ;
    sprites[N_EAST][3].y = 227 ;
    for ( i =  0 ; i < 4 ; ++i )
    {
        sprites[N_WEST][i].y = 41 ;
    }
    sprites[N_WEST][0].x = 356;
    sprites[N_WEST][1].x = 324 ;
    sprites[N_WEST][2].x = 292 ;
    sprites[N_WEST][3].x = 101 ;
    sprites[N_WEST][3].y = 173 ;
    for ( i =  0 ; i < 4 ; ++i )
    {
        sprites[S_EAST][i].y = 71 ;
    }
    sprites[S_EAST][0].x = 291;
    sprites[S_EAST][1].x = 326 ;
    sprites[S_EAST][2].x = 355 ;
    sprites[S_EAST][3].x = 165 ;
    sprites[S_EAST][3].y = 196 ;
    for ( i =  0 ; i < 4 ; ++i )
    {
        sprites[S_WEST][i].y = 7 ;
    }
    sprites[S_WEST][0].x = 356;
    sprites[S_WEST][1].x = 324 ;
    sprites[S_WEST][2].x = 292 ;
    sprites[S_WEST][3].x = 165 ;
    sprites[S_WEST][3].y = 132 ;
}

void pause(SDL_Surface **screenPtr)
{
    temp = SDL_ConvertSurface(*screenPtr, (*screenPtr)->format, (*screenPtr)->flags);
    SDL_Rect p ;
    p.x = 190;
    p.y = 200;
    SDL_BlitSurface( Pause, NULL, *screenPtr, &p);
    SDL_Flip(*screenPtr);
}

int gameRun( SDL_Surface **screenPtr, FMOD_SYSTEM **sysPtr, int lvl, int *run)
{
    setFrames();
    if ( TTF_Init() == -1 )
    {
        fprintf(stderr,"%s\n",TTF_GetError());
        return EXIT_FAILURE ;
    }
    police = TTF_OpenFont("loveMe.ttf",40);
    if ( police == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    policeNumber = TTF_OpenFont("loveMe.ttf",25);
    if ( policeNumber == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    Pause = TTF_RenderText_Blended(police,"pause",BLACK);
    youWin = TTF_RenderText_Blended(police,"You Win !",RED);
    if ( youWin == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    youLose = TTF_RenderText_Blended(police,"You Lose !",RED);
    if ( youLose == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    tryAgain = TTF_RenderText_Blended(police,"Try again ?",RED);
    if ( tryAgain == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    score = TTF_RenderText_Blended(policeNumber,"Number of moves :",BLUE);
    if ( score == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    reset = TTF_RenderText_Blended(policeNumber,"Reset",BLACK);
    if ( reset == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    p.x = 140;
    p.y = 20;
    p1.x = 140 ;
    p1.y = 100 ;
    p2.x = 140 ;
    p2.y = 220 ;
    p3.x = 260 ;
    p3.y = 220 ;
    p4.x = 240 ;
    p4.y = 430 ;
    p5.x = 10 ;
    p5.y = 430 ;
    p6.x = 380 ;
    p6.y = 430 ;
    yellow = IMG_Load("image/yellow.bmp");
    if ( yellow == NULL )
    {
        fprintf(stderr, "IMG_Load : %s\n",SDL_GetError());
        return EXIT_FAILURE ;
    }
    green = IMG_Load("image/green.bmp");
    if ( green == NULL )
    {
        fprintf(stderr, "IMG_Load : %s\n",SDL_GetError());
        return EXIT_FAILURE ;
    }
    cat = IMG_Load("image/cats.png");
    if ( cat == NULL )
    {
        fprintf(stderr, "IMG_Load : %s\n",SDL_GetError());
        return EXIT_FAILURE ;
    }
    ok_button = IMG_Load("image/ok.png");
    if ( ok_button == NULL )
    {
        fprintf(stderr, "IMG_Load : %s\n",SDL_GetError());
        return EXIT_FAILURE ;
    }
    ko_button = IMG_Load("image/ko.png");
    if ( ko_button == NULL )
    {
        fprintf(stderr, "IMG_Load : %s\n",SDL_GetError());
        return EXIT_FAILURE ;
    }
    if (FMOD_System_CreateSound(*sysPtr, "sound/laugh_cat.wav", FMOD_CREATESAMPLE, 0, &laugh_cat) != FMOD_OK )
    {
        fprintf(stderr, "Error : CreateSound!\n");
        return(EXIT_FAILURE);
    }
    if (FMOD_System_CreateSound(*sysPtr, "sound/angry_cat.wav", FMOD_CREATESAMPLE, 0, &angry_cat) != FMOD_OK )
    {
        fprintf(stderr, "Error : CreateSound!\n");
        return(EXIT_FAILURE);
    }
    if (FMOD_System_CreateSound(*sysPtr, "sound/meow_short.wav", FMOD_CREATESAMPLE, 0, &jump) != FMOD_OK )
    {
        fprintf(stderr, "Error : CreateSound!\n");
        return(EXIT_FAILURE);
    }
    if (FMOD_System_CreateSound(*sysPtr, "sound/mouseClick.wav", FMOD_CREATESAMPLE, 0, &click) != FMOD_OK )
    {
        fprintf(stderr, "Error : CreateSound!\n");
        return(EXIT_FAILURE);
    }
    if ( FMOD_System_CreateSound(*sysPtr,"sound/Maddeford.mp3"   , FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &music) != FMOD_OK )
    {
        fprintf(stderr,"Error : CreateSound!\n");
        return(EXIT_FAILURE);
    }
    FMOD_Sound_SetLoopCount(music, -1);
    FMOD_System_PlaySound( *sysPtr, FMOD_CHANNEL_FREE, music, 0, NULL);
    SDL_Event event ;
    initGame(screenPtr, lvl);
    continuer = 1 ;
    while ( continuer == 1  )
    {
        SDL_PollEvent(&event) ;
        switch ( event.type )
        {
            case SDL_QUIT:
                continuer = 0 ;
                *run = 0 ;
                break ;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_p:
                        event.key.type = SDL_KEYUP;
                        event.key.state = SDL_RELEASED;
                        if ( paused == 0 )
                        {
                            paused = 1 ;
                            pause(screenPtr);
                            continue ;
                        }
                        else
                        {
                            paused = 0 ;
                            unpause(screenPtr);
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if ( paused == 0 )
                {
                    switch( event.button.button )
                    {
                        case SDL_BUTTON_LEFT:
                            x =  event.motion.y / CIRCLE_HEIGHT;
                            if ( ( x + 1 ) % 2 == 0)
                                y = (event.motion.x - (CIRCLE_WIDTH / 2)) / CIRCLE_WIDTH ;
                            else
                                y = event.motion.x / CIRCLE_WIDTH ;
                            if ( (y >= 0) && (y<= 10) &&  ( x >= 0 ) && ( x <= 10 ) )
                            {
                                if ( (map[x][y].etat != GREEN ) && (x != catPos.x || y!= catPos.y ) )
                                {
                                    FMOD_System_PlaySound( *sysPtr, FMOD_CHANNEL_FREE, click, 0, NULL);
                                    chaseCat(x, y, lvl);
                                    validClick = 1 ;
                                    ++nbClick;
                                }
                            }
                            else if ( (event.motion.x >= p6.x) && (event.motion.x <= p6.x + reset->w ) && (event.motion.y >= p6.y ) && ( event.motion.y <= p6.y + reset->h ))
                            {
                                initGame(screenPtr, lvl);
                            }
                            break ;
                    }
                    break;
                }
        }
        if ( paused == 0 )
        {
            precedent = actuel ;
            actuel = SDL_GetTicks();
            if( 30 - (actuel - precedent) > 0 )
                SDL_Delay(30 - (actuel - precedent ));
            if(validClick == 1 )
            {
                validClick = 0 ;
                oldCatPos = catPos;
                if (existWay() == 1 )
                {
                    moveCat(lvl);
                    FMOD_System_PlaySound( *sysPtr, FMOD_CHANNEL_FREE, jump, 0, NULL);
                }
                playAnim(screenPtr);
            }
            if ( gameOver() )
            {
                a = 0 ;
                if(catPos.x ==0 || catPos.y == 0 || catPos.x == MAP_SIZE - 1 ||  catPos.y == MAP_SIZE-1 )
                {
                    SDL_BlitSurface( youLose, NULL, *screenPtr, &p );
                    SDL_BlitSurface( tryAgain, NULL, *screenPtr, &p1 );
                    SDL_BlitSurface( ok_button, NULL, *screenPtr, &p2 );
                    SDL_BlitSurface( ko_button, NULL, *screenPtr, &p3 );
                    FMOD_System_PlaySound( *sysPtr, FMOD_CHANNEL_FREE, laugh_cat, 0, NULL);
                }
                else
                {
                    scorE(lvl+1,nbClick);
                    SDL_BlitSurface( youWin, NULL, *screenPtr, &p );
                    SDL_BlitSurface( tryAgain, NULL, *screenPtr, &p1 );
                    SDL_BlitSurface( ko_button, NULL, *screenPtr, &p3 );
                    SDL_BlitSurface( ok_button, NULL, *screenPtr, &p2 );
                    FMOD_System_PlaySound( *sysPtr, FMOD_CHANNEL_FREE, angry_cat, 0, NULL);
                }
                SDL_Flip(*screenPtr);
                while ( a == 0 )
                {
                    SDL_WaitEvent(&event);
                    switch(event.type)
                    {
                        case SDL_QUIT:
                            a = 1 ;
                            continuer = 0 ;
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            if ( (event.motion.x <= (60 + p2.x)) && (event.motion.x >= p2.x) && (event.motion.y <= (60 + p2.y)) && (event.motion.y >= p2.y) ) 
                            {
                                a = 1 ;
                                continuer = 1 ;
                                initGame(screenPtr, lvl);
                                event.button.type = SDL_MOUSEBUTTONUP ;
                                event.button.state = SDL_RELEASED ;
                            }
                            else if ( (event.motion.x <= (60 + p3.x)) && (event.motion.x >= p3.x) && (event.motion.y <= (60 + p3.y)) && (event.motion.y >= p3.y) )
                            {
                                a = 1 ;
                                continuer = 0 ;
                            }
                            break ;
                    }
                }
            }       


        }
    }
    FMOD_Sound_Release(jump);
    FMOD_Sound_Release(click);
    FMOD_Sound_Release(laugh_cat);
    FMOD_Sound_Release(angry_cat);
    FMOD_Sound_Release(music);
    TTF_Quit();
    SDL_FreeSurface(Pause);
    SDL_FreeSurface(reset);
    SDL_FreeSurface(score);
    SDL_FreeSurface(CLICK);
    SDL_FreeSurface(youWin);
    SDL_FreeSurface(youLose);
    SDL_FreeSurface(tryAgain);
    SDL_FreeSurface(ko_button);
    SDL_FreeSurface(ok_button);
    SDL_FreeSurface(yellow);
    SDL_FreeSurface(green);
    SDL_FreeSurface(cat);
    SDL_FreeSurface(temp);
    return EXIT_SUCCESS ;
}

int howTo( SDL_Surface **screenPtr)
{
    setFrames();
    if ( TTF_Init() == -1 )
    {
        fprintf(stderr,"%s\n",TTF_GetError());
        return EXIT_FAILURE ;
    }
    police = TTF_OpenFont("loveMe.ttf",40);
    if ( police == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    policeNumber = TTF_OpenFont("loveMe.ttf",25);
    if ( policeNumber == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    Pause = TTF_RenderText_Blended(police,"pause",BLACK);
    youWin = TTF_RenderText_Blended(police,"You Win !",RED);
    if ( youWin == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    youLose = TTF_RenderText_Blended(police,"You Lose !",RED);
    if ( youLose == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    tryAgain = TTF_RenderText_Blended(police,"Try again ?",RED);
    if ( tryAgain == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    score = TTF_RenderText_Blended(policeNumber,"Number of moves :",BLUE);
    if ( score == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    reset = TTF_RenderText_Blended(policeNumber,"Reset",BLACK);
    if ( reset == NULL )
    {
        fprintf(stderr,"TTF_OpenFont : %s\n",TTF_GetError());
        return EXIT_FAILURE;
    }
    p.x = 140;
    p.y = 20;
    p1.x = 140 ;
    p1.y = 100 ;
    p2.x = 140 ;
    p2.y = 220 ;
    p3.x = 260 ;
    p3.y = 220 ;
    p4.x = 240 ;
    p4.y = 430 ;
    p5.x = 10 ;
    p5.y = 430 ;
    p6.x = 380 ;
    p6.y = 430 ;
    yellow = IMG_Load("image/yellow.bmp");
    if ( yellow == NULL )
    {
        fprintf(stderr, "IMG_Load : %s\n",SDL_GetError());
        return EXIT_FAILURE ;
    }
    green = IMG_Load("image/green.bmp");
    if ( green == NULL )
    {
        fprintf(stderr, "IMG_Load : %s\n",SDL_GetError());
        return EXIT_FAILURE ;
    }
    cat = IMG_Load("image/cats.png");
    if ( cat == NULL )
    {
        fprintf(stderr, "IMG_Load : %s\n",SDL_GetError());
        return EXIT_FAILURE ;
    }
    initGame(screenPtr, HARD);
    while ( ! gameOver() )
    {
        do 
        {
            x = rand()%MAP_SIZE ;
            y = rand()%MAP_SIZE ;
        } while ( map[x][y].etat == GREEN || (x==catPos.x && y==catPos.y))  ;
        chaseCat(x, y, HARD);
        precedent = actuel ;
        actuel = SDL_GetTicks();
        if( 30 - (actuel - precedent) > 0 )
            SDL_Delay(30 - (actuel - precedent ));
        oldCatPos = catPos;
        if (existWay() == 1 )
        {
            moveCat(HARD);
        }
        playAnim(screenPtr);
        SDL_Delay(500);
    }
    if ( gameOver() )
    {
        SDL_BlitSurface( youLose, NULL, *screenPtr, &p );
        SDL_Flip(*screenPtr);
        SDL_Delay(500);
    }       

    initGame(screenPtr, EASY);
    map[5][6].etat=map[4][6].etat=map[4][5].etat=map[4][4].etat=map[5][6].etat=map[6][6].etat=map[6][5].etat=map[6][4].etat=GREEN ;
    chaseCat(5, 3, EASY);
    moveCatNormal();
    playAnim(screenPtr);
    SDL_Delay(500);
    oldCatPos = catPos ;
    chaseCat(5,5, EASY);
    playAnim(screenPtr);
    SDL_Delay(500);
    if ( gameOver() )
    {
        SDL_BlitSurface( youWin, NULL, *screenPtr, &p );
        SDL_Flip(*screenPtr);
        SDL_Delay(500);
    }
    TTF_Quit();
    SDL_FreeSurface(Pause);
    SDL_FreeSurface(youWin);
    SDL_FreeSurface(youLose);
    SDL_FreeSurface(tryAgain);
    SDL_FreeSurface(yellow);
    SDL_FreeSurface(green);
    SDL_FreeSurface(cat);
    SDL_FreeSurface(temp);
    return EXIT_SUCCESS ;
}
