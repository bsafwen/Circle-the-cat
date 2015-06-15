#include "linkedList.h"

int main()
{
    list l = NULL ;
    int i ;
    for( i = 0 ; i < 20 ; ++i )
    {
        inserer(&l, i+1);
    }
    afficher(&l);
    while ( l != NULL )
        delete(&l);
    return 0 ;
}
