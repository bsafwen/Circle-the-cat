#ifndef LIN_H
#define LIN_H

#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *nxt;
};

typedef struct node Node;
typedef Node *list;

void afficher(const list *l);
void inserer(list *l,int c);
int estVide(const list *l);
int longueur(const list *l);
void delete(list *l);

#endif
