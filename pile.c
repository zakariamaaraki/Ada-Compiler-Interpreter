#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

int pile_vide(Pile  p){
	return (p.sommet == -1);
}

void empiler(Pile* p, double x){
	p->elements[p->sommet + 1] = x;
	p->sommet ++;
}

// pré-condition : pile_vide(p) = false
double depiler(Pile* p){
	return p->elements[p->sommet--];
}

double tetepile(Pile p){
	return p.elements[p.sommet];
}