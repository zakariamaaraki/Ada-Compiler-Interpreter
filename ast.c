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
  AST result = (AST)malloc(sizeof(expvalueType));
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

AST creer_feuille_idfArray(char* nameArray, AST exp){
  AST result = (AST) malloc (sizeof(expvalueType));
  result->typeexp=Array;
  result->typename = 0; // Int ou Float ou Double
  result->noeud.op.expression_droite = exp; 
  strcpy(result->noeud.idf, nameArray);
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
  static int supLabel=0;
  static int infLabel=0;
  static int egLabel=0;

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

    case supEg : 
      supLabel++;
      fprintf(file,"SWAP \n");
      fprintf(file,"SUB \n"); // opération non commutative
      fprintf(file,"PUSH 0.000000\n");
      fprintf(file,"SWAP\n");
      fprintf(file,"JGE supLabelTrue%d\n",supLabel); // opération non commutative
      fprintf(file,"PUSH 0.000000\n");
      fprintf(file,"JMP supLabelFalse%d\n",supLabel);
      fprintf(file,"LABEL supLabelTrue%d\n",supLabel);
      fprintf(file,"PUSH 1.000000\n");
      fprintf(file,"LABEL supLabelFalse%d\n",supLabel);	
      break;  

    case sup : 
      supLabel++;
      fprintf(file,"SWAP \n");
      fprintf(file,"SUB \n"); // opération non commutative
      fprintf(file,"PUSH 0.000000\n");
      fprintf(file,"SWAP\n");
      fprintf(file,"JG supLabelTrue%d\n",supLabel); // opération non commutative
      fprintf(file,"PUSH 0.000000\n");
      fprintf(file,"JMP supLabelFalse%d\n",supLabel);
      fprintf(file,"LABEL supLabelTrue%d\n",supLabel);
      fprintf(file,"PUSH 1.000000\n");
      fprintf(file,"LABEL supLabelFalse%d\n",supLabel);	
      break;

    case inf : 
      infLabel++;
      fprintf(file,"SWAP \n");
      fprintf(file,"SUB \n"); // opération non commutative
      fprintf(file,"PUSH 0.000000\n");
      fprintf(file,"SWAP\n");
      fprintf(file,"JGE infLabelTrue%d\n",infLabel); // opération non commutative
      fprintf(file,"PUSH 1.000000\n");
      fprintf(file,"JMP infLabelFalse%d\n",infLabel);
      fprintf(file,"LABEL infLabelTrue%d\n",infLabel);
      fprintf(file,"PUSH 0.000000\n");
      fprintf(file,"LABEL infLabelFalse%d\n",infLabel);	
      break; 

    case infEg : 
      infLabel++;
      fprintf(file,"SWAP \n");
      fprintf(file,"SUB \n"); // opération non commutative
      fprintf(file,"PUSH 0.000000\n");
      fprintf(file,"SWAP\n");
      fprintf(file,"JG infLabelTrue%d\n",infLabel); // opération non commutative
      fprintf(file,"PUSH 1.000000\n");
      fprintf(file,"JMP infLabelFalse%d\n",infLabel);
      fprintf(file,"LABEL infLabelTrue%d\n",infLabel);
      fprintf(file,"PUSH 0.000000\n");
      fprintf(file,"LABEL infLabelFalse%d\n",infLabel);	
      break;  

      case eg : 
      egLabel++;
      fprintf(file,"JNE egLabelTrue%d\n",egLabel); // opération non commutative
      fprintf(file,"PUSH 1.000000\n");
      fprintf(file,"JMP egLabelFalse%d\n",egLabel);
      fprintf(file,"LABEL LabelTrue%d\n",egLabel);
      fprintf(file,"PUSH 0.000000\n");
      fprintf(file,"LABEL egLabelFalse%d\n",egLabel);	
      break;  
    }
    break;

    case Array :
      generer_pseudo_code_ast(arbre_droit(ast),file);  
      fprintf(file,"LOADARRAY %s \n",ast->noeud.idf);
    break;

  }
 
}