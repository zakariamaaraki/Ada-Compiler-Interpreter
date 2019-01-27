#include "ast.h"


typedef enum {Exit, PrintExp, Message, DECL, PrintIdf, PrintString, AssignArith, IfThenArith, IfThenElseArith, For, Loop, While, Goto} Type_INST ;

typedef struct INST {
  Type_INST typeinst;
  union  {
    // PRINT idftoprint
    struct  {
      int rangvar; // indice de l'idf, à afficher, dans la table des symboles
    } printnode;
    // MESSAGE
    struct  {
      char message[30]; 
    } messagenode;
    // PRINT EXP
    struct  {
      AST exp;
    }printExpnode;
    // left := right
    struct  {
      int rangvar; // indice de l'idf, où il faut affecter, dans la table des symboles
      AST right; // l'expression à affecter (arithmétique ou booléenne)
    } assignnode;
    // IF ... THEN 
    struct  {
      //int rangvar; // indice de l'idf à comparer, dans la table des symbole
      AST right; // l'expression à comparer
      struct LIST_INST * thenlinst; // then list of instructions
      struct LIST_INST * elselinst; // else list of instructions
    } ifnode;
    // IF ... THEN 
      //for
    struct {
      int rangvar; // indice de l'index de la boucle
      AST borneinf; // l'expression borne inf
      AST bornesup; // l'expression borne sup
      int reverse; //0 or 1
      struct LIST_INST * forbodylinst; // for body list of instructions
    } fornode;
    //while	
    struct {
      AST cond; // indice de l'index de la boucle
      struct LIST_INST * whilebodylinst; // for body list of instructions
    } whilenode;
    //loop
    struct {
      struct LIST_INST * loopbodylinst;	
    }loopnode;
    //goto
    struct {
    	char label[30];
    }gotonode;
    //decl
    struct {
    	char name[30];
    	double value;
    }declnode;
    //exit
    struct {
    	AST cond;
    }exitnode;

  } node;
} instvalueType;


typedef struct LIST_INST {
  struct INST first;
  struct LIST_INST * next;
} listinstvalueType;

instvalueType* creer_instruction_print(int rangvar);
instvalueType* creer_instruction_message(char* message);
instvalueType* creer_instruction_print_exp(AST ast);
instvalueType* creer_instruction_exit(AST cond);
instvalueType* creer_instruction_decl(double value, char* name);
instvalueType* creer_instruction_affectation(int rangvar, AST past);
instvalueType* creer_instruction_if( AST past, listinstvalueType* plistthen, listinstvalueType* plistelse);
instvalueType* creer_instruction_for(int rangvar, AST borneinf, AST bornesup, int reverse, listinstvalueType* pplistfor);
instvalueType* creer_instruction_while(AST cond, listinstvalueType* pplistwhile);
instvalueType* creer_instruction_loop(listinstvalueType* pplistloop);
instvalueType* creer_instruction_goto(char* label);

void inserer_inst_en_queue(listinstvalueType* pp, instvalueType p);

void generer_pseudo_code_inst(instvalueType p,FILE* file);
void generer_pseudo_code_list_inst(listinstvalueType* p, FILE* file);
