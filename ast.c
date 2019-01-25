#include "pseudoCode.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// précondition : a<> NULL and est_feuille(a) == false
AST arbre_gauche(AST a){
  return a->noeud.op.expression_gauche;
}

// précondition : a<> NULL and est_feuille(a) == false
AST arbre_droit(AST a){
  return a->noeud.op.expression_droite;
}

// précondition : a<> NULL and est_feuille(a) == false
Type_Op top(AST a){
  return a->noeud.op.top;
}

// précondition : a<> NULL
Type  type(AST a){
return a->typename;
}

// précondition : a<> NULL 
int est_feuille(AST a){
  return (a->typeexp != OP) ;
}

AST creer_feuille_nombre(double n, Type type){
  AST result = (AST) malloc (sizeof(expvalueType));
  result->typeexp=NB;
  result->noeud.nombre = n;
  result->typename = type;
  return result;
}

AST creer_feuille_idf(char *idf, Type type){
  AST result = (AST) malloc (sizeof(expvalueType));
  result->typeexp=_IDF;
  result->typename = type; // Int ou Float ou Double
  strcpy(result->noeud.idf, idf);
  return result;
}

AST creer_noeud_operation(char op, AST arbre_g, AST arbre_d, Type type){
  AST result= (AST) malloc (sizeof(expvalueType));
  result->typeexp=OP;
  result->typename = type;
  result->noeud.op.top = type;
  result->noeud.op.expression_gauche = arbre_g;
  result->noeud.op.expression_droite = arbre_d;
  return result;
}

// génère le pseudo-code relatif à l'AST
// précondition ast <> NULL
 void generer_pseudo_code_ast(AST ast, FILE* file){

  switch(ast->typeexp) {
  case NB :
    fprintf(file,"PUSH %lf\n",ast->noeud.nombre);
    break;

  case _IDF :
    fprintf(file,"LOAD %s\n",ast->noeud.idf);
    break;

  case OP :   
    generer_pseudo_code_ast(arbre_gauche(ast),file);
    generer_pseudo_code_ast(arbre_droit(ast),file);   

    switch(ast->noeud.op.top){
    case plus :
      fprintf(file,"ADD \n");
      break;

    case minus :
      fprintf(file,"SWAP \n");
      fprintf(file,"SUB \n"); // opération non commutative
      break;

    case mult : 
      fprintf(file,"MULT \n");
      break;

    case _div : 
      fprintf(file,"SWAP \n");
      fprintf(file,"DIV \n"); // opération non commutative
      break;
    }

  }
 
}