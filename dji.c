#include "linkedList.h"
#include "game.h"
#include "dji.h"

#define IN 10000
#define SOMMET 121

Rect dji(int cost[][SOMMET], int source, int target )
{
    int dist[SOMMET], prev[SOMMET], min = IN, alt, u, v, ind = 1 ;
    list listee = NULL, liste = NULL, temp = NULL ;
    Rect tmp;
    tmp.x = tmp.y = tmp.h = -IN ;
    int n = 0 ;
    dist[source] = 0 ;
    prev[source] = -1 ;
    for ( v =0 ; v < SOMMET ; ++v )
    {
        if ( v != source )
        {
            dist[v] = IN ;
            prev[v] = -1 ;
        }
        insert(&listee,v,ind++);
    }
    liste = temp  = listee ;
    ind = 1 ;
    while ( liste != NULL )
    {
        ++n ;
        list neighbours = NULL ;
        temp = liste ;
        min = IN ;
        while ( temp != NULL )
        {
            if ( dist[temp->value] < min )
            {
                min = dist[temp->value] ;
                u = temp->value ; 
            }
            temp=temp->nxt ;
        }
        temp = liste;
        supprimer(&liste, u);
        if ( liste == NULL || u == target || n > SOMMET - 1 )
        {
            while ( listee != NULL )
                delete(&listee,1);
            break ;
        }
        temp = liste ;
        ind = 1 ;
        while ( temp != NULL )
        {
            if ( cost[u][temp->value] < IN )
            {
                insert(&neighbours,temp->value,ind++);
            }
            temp = temp->nxt ;
        }
        temp = neighbours ;
        while ( temp != NULL )
        {
            alt = dist[u] + cost[u][temp->value] ;
            if ( alt < dist[temp->value] )
            {
                dist[temp->value] = alt ;
                prev[temp->value] = u ;
            }
            temp = temp->nxt ;
        }
        while ( neighbours != NULL )
            delete(&neighbours,1);
    }
    u = target ;
    tmp.x = u / MAP_SIZE ;
    tmp.y = u % MAP_SIZE ;
    tmp.h = dist[target];
    while ( prev[u] != -1 )
    {
        u = prev[u] ;
        if ( u == source ) break ;
        tmp.x = u / MAP_SIZE ;
        tmp.y = u % MAP_SIZE ;
    }
    while ( listee != NULL )
        delete(&listee,1);
    return tmp ;
}

Rect miin(Rect t[], int n)
{
    int i, min = IN*IN,m;
    for ( i = 0 ; i < n ; ++i )
        if ( t[i].h < min )
        {
            min = t[i].h ;
            m= i ;
        }
    return t[m];
}
