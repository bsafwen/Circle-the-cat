#include "init_quit.h"

void initSDL(SDL_Surface **screenPtr)
{
    if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
        fprintf(stderr,"Init : %s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }
    *screenPtr = SDL_SetVideoMode( 460, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT );
    if ( *screenPtr == NULL )
    {
        fprintf(stderr, "SetVideoMode : %s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void initFMOD(FMOD_SYSTEM **sys)
{
    if( FMOD_System_Create(sys) != FMOD_OK )
    {   
        fprintf(stderr,"Error : System_Create !\n");
        exit(EXIT_FAILURE);
    }
    if (FMOD_System_Init( *sys, 32, FMOD_INIT_NORMAL, NULL ) != FMOD_OK )
    {   
        fprintf(stderr,"Error : System_Init !\n");
        exit(EXIT_FAILURE);
    }
}

void cleanUp( FMOD_SYSTEM **sys, SDL_Surface **img1, SDL_Surface **img2)
{
    SDL_FreeSurface(*img1);
    SDL_FreeSurface(*img2);
    FMOD_System_Close(*sys);
    FMOD_System_Release(*sys);
    SDL_Quit();
}
