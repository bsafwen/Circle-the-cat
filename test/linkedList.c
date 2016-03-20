#include "linkedList.h"

int estVide(const list (*l))
{
    if ( (*l) == NULL )
        return 0 ;
    return 1 ;
}

void afficher(const list (*l))
{
    if ( *l != NULL )
    {
        list temp = *l ;
        while ( temp != NULL )
        {
            printf("%d  -  ",temp->value);
            temp = temp->nxt ;
        }
    }
}

void inserer(list (*l),int c)
{
    list new = malloc(sizeof(Node));
    new->value = c ;
    new->nxt =*l ;
    *l = new ;
}
int longueur(const list (*l))
{
    if ( estVide(l) )
        return 0 ;
    else
    {
        int i = 1 ;
        list temp = *l ; 
        while ( temp->nxt != NULL )
        {
            ++i ;
            (temp) = (temp)->nxt ;
        }
        return i ;
    }
}
void delete(list (*l))
{
    list toDelete = (*l) ;
    (*l) = (*l)->nxt ;
    free(toDelete);
}



