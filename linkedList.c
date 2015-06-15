#include "linkedList.h"

int length(const list *l)
{
    list temp = *l ;
    int i = 0 ;
    while ( temp != NULL )
    {
        ++i ;
        temp = temp->nxt ;
    }
    return i ;
}

void display(const list *l)
{
    list temp = *l ;
    while ( temp != NULL )
    {
        printf("%d  -  ", temp->value );
        temp = temp->nxt ;
    }
    printf("\n");
}

void delete(list *l, int n)
{
    if( n >= 1  && n <= length(l) )
    {
        if ( n == 1 )
        {
            list temp = *l ;
            *l = (*l)->nxt ;
            free(temp);
        }
        else
        {
            list temp = *l ;
            int i = 1 ;
            for ( i = 1 ; i <= n - 2 ; ++i )
                temp=temp->nxt ;
            list toDelete = temp->nxt ;
            list next = temp->nxt ;
            temp->nxt = next->nxt ;
            free(toDelete);
        }
    }
}

void insert(list *l, int n, int pos)
{
    if ( *l == NULL )
    {
        list new = malloc(sizeof(Node));
        new->value = n ;
        new->nxt = NULL ;
        *l = new ;
    }
    else if ( pos >= 1 && pos <= length(l)+1)
    {
        list new = malloc(sizeof(Node));
        new->value = n ;
        new->nxt = NULL ;
        int i =1 ;
        list temp = *l ;
        for ( i = 1 ; i <= pos - 2 ; ++i )
            temp=temp->nxt;
        new->nxt = temp->nxt;
        temp->nxt = new ;
    }
    else
        printf("insertion : invalid position.\n");
}

void clean(list *l)
{
    while ( *l != NULL )
        delete(l,1);
}

void supprimer(list *l,int c)
{
    if(*l == NULL)
    {
        puts("La liste est deja vide.\n");
    }
    else {
        list courant=*l;
        list preced,temp;

        if(courant->value==c){
            temp=*l;
            *l=(*l)->nxt;
            temp=NULL;
            free(temp);
        } 
        else {
            while((courant!=NULL)&&(courant->value!=c)){
                preced=courant;
                courant=courant->nxt;
            }
            if(courant!=NULL){
                temp=courant;
                preced->nxt=courant->nxt;
                courant=courant->nxt;
                preced=courant=temp=NULL;
                free(temp);
                free(preced);
                free(courant);
            }
            /* else */
            /* { */
                /* printf("%d est non trouvé!\n",c); */
            /* } */
        }
    }
}


