#include "cTc.h"

Coordinates chat ;
Map map ;

main()
{
    chat.x = MAP_SIZE/2;
    chat.y = MAP_SIZE/2;
    initializeMap(&map,&chat);
    draw(&map);
    chaseCat(&map);
    firstStep(&map, &chat);
    draw(&map);
    while( !gameOver(&map, &chat) )
    {
        chaseCat(&map);
        if(existWay(&chat,&map))
        {
            moveCat(&map, &chat);
        }
        draw(&map);
    }
    if(chat.x ==0 || chat.y == 0 || chat.x == MAP_SIZE - 1 ||  chat.y == MAP_SIZE-1 )
        printf("The cat has escaped! You Lose\n");
    else
        printf("Well done, You catched the cat!\n");
    quitGame(&map);
}
