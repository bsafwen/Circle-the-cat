#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "game.h"
#include "init_quit.h"

const SDL_Color black = { 0, 0, 0 } ;

void mainMenu(void);
void levelMenu(void);
void high_score(int *run);

int clickHighScore(Uint16 x, Uint16 y);
int clickHowToPlay(Uint16 x, Uint16 y) ;
int clickExit(Uint16 x, Uint16 y) ;

int clickPlay(Uint16 x, Uint16 y);
int clickLevel(Uint16 x, Uint16 y);
SDL_Surface* screen=NULL, *SCORE[9] ;
SDL_Surface *img_menu = NULL, *img_level=NULL, *img_highScore = NULL  ;
SDL_Rect dstrect ;
FMOD_SYSTEM *sys = NULL ;
int level = NORMAL, scoree[9]={0} ;
int running ;
int soundon ;

int main( int argc, char *argv[])
{

    running=1 ;
    soundon= 1 ;
    initFMOD(&sys);
    initSDL(&screen);
    img_menu = SDL_LoadBMP("image/menu.bmp");
    if (!img_menu)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }
    dstrect.x = 0;
    dstrect.y = 0;
    img_level = SDL_LoadBMP("image/Level.bmp");
    if (!img_level)
    {
        printf("Unable to load bitmape: %s\n", SDL_GetError());
        return 1;
    }
    img_highScore = SDL_LoadBMP("image/highscore.bmp");
    if (!img_highScore)
    {
        printf("Unable to load bitmape: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Event event ;
    SDL_WM_SetCaption("circle The cat",NULL);
    while ( running == 1)
    {
        mainMenu();
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                running = 0 ;
                break ;
            case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        running = 0;
                    break;
                }
            case SDL_MOUSEBUTTONDOWN:
                if ( clickLevel(event.button.x,event.button.y)==1)
                {
                    levelMenu();
                }
                else if ( clickPlay(event.button.x, event.button.y) == 1)
                    gameRun(&screen, &sys, level, &running);
                else if (clickExit(event.button.x, event.button.y)==1)
                {
                    running = 0 ;
                }
                else if (clickHighScore(event.button.x, event.button.y)==1)
                {
                    high_score(&running);
                }
                else if (clickHowToPlay(event.button.x, event.button.y)==1)
                {
                    howTo(&screen);
                }
                break;
        }
    }
    cleanUp(&sys, &img_menu, &img_level);
    return EXIT_SUCCESS ;
}
void levelMenu(void)
{
    SDL_Event event ;
    int continuer = 1 ;
    while ( continuer == 1 )
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                {
                    event.button.type = SDL_MOUSEBUTTONUP;
                    event.button.state = SDL_RELEASED;
                    if (event.button.x<=397 && event.button.x>=60 && event.button.y<=290 && event.button.y>=250)
                    {
                        level=EASY ;
                        continuer=0;
                    }
                    else if  (event.button.x<=397 && event.button.x>=60 && event.button.y<=340 && event.button.y>=300)
                    {
                        level=NORMAL ;
                        continuer=0;
                    }
                    else if   (event.button.x<=397 && event.button.x>=60 && event.button.y<=390 && event.button.y>=350)
                    {
                        level=HARD ;
                        continuer=0;
                    }
                }
                break ;
            case SDL_QUIT:
                running = 0 ;
                continuer=0 ;
                break ;
            case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        continuer = 0;
                }
                break;
        }
        SDL_BlitSurface(img_level, 0, screen, &dstrect) ;
        SDL_Flip(screen);
    }
}

int clickLevel(Uint16 x, Uint16 y)
{
    if (x<=397 && x>=60 && y<=290 && y>=250 )
    {
        return 1;
    }
    return 0 ;
}


int clickPlay(Uint16 x, Uint16 y)
{
    if (x<=397 && x>=60 && y<=240 && y>=200 )
    {
        return 1;
    }
    return 0 ;
}

void mainMenu(void)
{
    SDL_BlitSurface(img_menu, 0, screen, &dstrect) ;
    SDL_Flip(screen);
}

int clickExit(Uint16 x, Uint16 y)
{
    if (x>=60 && x<=397 && y>=400 && y<=440)
        return 1 ;
    return 0 ;
}

int clickHighScore(Uint16 x, Uint16 y)
{
    if (x>=60 && x<=397 && y>=350 && y<=390)
        return 1 ;
    return 0 ;
}

int clickHowToPlay(Uint16 x, Uint16 y)
{
    if (x>=60 && x<=397 && y>=300 && y<=340)
        return 1 ;
    return 0 ;

}

void high_score(int *run)
{
    TTF_Init();
    TTF_Font *police = NULL ;
    police = TTF_OpenFont("loveMe.ttf",40);
    SDL_Event event ;
    int continuer = 1,i ;
    SDL_BlitSurface( img_highScore, NULL, screen, NULL);
    SDL_Flip(screen);
    FILE *f1 = NULL, *f2 = NULL, *f3  = NULL;
    f1 = fopen("highscore/Easy.txt", "r");
    f2 = fopen("highscore/Medium.txt","r");
    f3 = fopen("highscore/Hard.txt", "r");
    SDL_Rect p ;
    fscanf(f1, "%d %d %d", &scoree[0], &scoree[1], &scoree[2]);
    fscanf(f2, "%d %d %d", &scoree[3], &scoree[4], &scoree[5]);
    fscanf(f3, "%d %d %d", &scoree[6], &scoree[7], &scoree[8]);
    char nc[3] ={'9'};
    for ( i = 0 ; i < 9 ; ++i )
    {
        if ( scoree[i] != 999 )
        {
            sprintf(nc,"%d",scoree[i]);
            SCORE[i] = TTF_RenderText_Blended(police,nc,black);
        }
    }
    p.x = 70 ;
    p.y = 250 ;
    for ( i = 0 ; i < 3 ; ++i )
    {
        if( scoree[i] != 999 )
        {
            SDL_BlitSurface(SCORE[i], NULL, screen, &p);
            p.y += 50 ;
        }
    }
    p.y = 250 ;
    p.x = 230 ;
    for ( i = 0 ; i < 3 ; ++i )
    {
        if( scoree[i+3] != 999 )
        {
            SDL_BlitSurface(SCORE[i+3], NULL, screen, &p);
            p.y += 50 ;
        }
    } 
    p.y = 250 ;
    p.x = 380 ;
    for ( i = 0 ; i < 3 ; ++i )
    {
        if( scoree[i+6] != 999 )
        {
            SDL_BlitSurface(SCORE[i+6], NULL, screen, &p);
            p.y += 50 ;
        }
    } 
    SDL_Flip(screen);
    while ( continuer == 1 )
    {
        SDL_WaitEvent(&event);
        if ( event.key.keysym.sym == SDLK_ESCAPE )
            continuer = 0 ;
        else if ( event.type == SDL_QUIT )
            continuer = *run = 0 ;
    }
    TTF_CloseFont(police);
    TTF_Quit();
    for ( i = 0 ; i < 9 ; ++i )
    {
        SDL_FreeSurface(SCORE[i]);
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
}
