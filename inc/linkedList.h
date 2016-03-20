#ifndef LList_H
#define LList_H

#include <stdio.h>
#include <stdlib.h>

struct node 
{
    int value ;
    struct node *nxt ;
};

typedef struct node Node ;
typedef Node * list ;

void insert(list *l, int n, int pos);
void delete(list *l, int pos);
void display(const list *l);
int length(const list *l);
void clean(list *l);
void supprimer(list *l, int value);

#endif
