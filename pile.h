
typedef struct{
	double elements[1000];
    int sommet;
}Pile;

// pré-condition : p <> NULL
int pile_vide(Pile p);

// pré-condition : p <> NULL
void empiler(Pile *p, double x);

// pré-condition : p <> NULL, pile_vide(p) = false
double depiler(Pile *p);

double tetepile(Pile p);
