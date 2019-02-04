#include <stdlib.h>
#include <stdio.h>


typedef enum {NB=0, _IDF = 1, OP=2, Array=3} Type_Exp ; // IDF is also a token !!

typedef enum {Int, _Float, _Double, _Boolean} Type;

typedef enum {plus, minus, mult, _div, sup, inf, eg, supEg, infEg, diff} Type_Op; // div est un mot réservé

struct Exp ; /* pré déclaration de la structure de stockage d'une expression */

typedef struct Exp* AST;

typedef union {
  double nombre ;
  char idf[30];
  struct {
    Type_Op top;
    AST expression_gauche ;
    AST expression_droite ;
  } op;
} ExpValueTypeNode;

typedef struct Exp {
  Type_Exp typeexp ; // NB ou IDF ou OP
  Type     typename; // Int ou Float ou Double
  ExpValueTypeNode noeud ;
}expvalueType;



// précondition : a<> NULL and est_feuille(a) == false
AST arbre_gauche(AST a);

// précondition : a<> NULL and est_feuille(a) == false
AST arbre_droit(AST a);

// précondition : a<> NULL and est_feuille(a) == false
Type_Op top(AST a);

// précondition : a<> NULL
Type type(AST a);

// précondition : a<> NULL 
int est_feuille(AST a);

AST creer_feuille_nombre(double n, Type type);

AST creer_feuille_idf(char *idf, Type type);

AST creer_noeud_operation(char op, AST arbre_g, AST arbre_d, Type type);

AST creer_feuille_idfArray(char* nameArray, AST exp);

void generer_pseudo_code_ast(AST ast, FILE* file);
