#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"
#include "types.h"
#include "pseudoCode.h"

FILE* file;
listinstvalueType* list_inst;
listinstvalueType* list_inst_if;
char nomToken[30];
char idfName[30];
namevalue TS[100];
int message=0;
int n=0;
AST ast=NULL;

double valeur(char* nomToken){
	int i=0;
	for(i=0;i<n;i++){
		if(strcmp(nomToken,TS[i].name)==0)return TS[i].value;
	}
	return -1;
}


int matchToken(int expected) {

  if(token.type != expected) {
    printf("** Syntax error, expecting: ");
    getMacro(expected);
    printf("** instead of: " );
    getMacro(token.type);
    exit(-1);
  }
  else{
  	strcpy(nomToken,token.val.stringValue);
  	scanToken();
  } 

  return 1;
}


void program() {
  with_use_clause();
  subprogram_body();
}


/*
  compilation_unit ::= context_clause ( library_item | subunit )

  context_clause ::= { ( with_clause | use_clause ) }

  library_item ::= [ "private" ] library_unit_declaration
                  | [ "private" ] library_unit_renaming_declaration
                  | library_unit_body

subprogram_specification ::= (library_unit_declaration ::=)
      ( "procedure" defining_program_unit_name [ formal_part ] )
      | ( "function" defining_designator [ formal_part ] "return" subtype_mark )

formal_part ::= "(" parameter_specification { ";" parameter_specification } ")"

*/



void with_use_clause() {
  switch (token.type) {
    case T_use: case T_with: scanToken(); break;
    default: return;
  }

  matchToken(T_IDENTIFIER);
  if(token.type == T_PT) {
      scanToken();
      matchToken(T_IDENTIFIER);
  }
  while(token.type == T_VIR) {
    scanToken();
    matchToken(T_IDENTIFIER);
    if(token.type == T_PT) {
        scanToken();
        matchToken(T_IDENTIFIER);
    }
  }
  matchToken(T_PV);
  switch (token.type) {
    case T_use: case T_with: with_use_clause();
  }
}



void subprogram_specification() {
  switch (token.type) {
    case T_procedure: scanToken(); matchToken(T_IDENTIFIER); if(token.type == T_PO) {matchToken(T_PO); formal_part(); matchToken(T_PF);} break;
    case T_function: scanToken(); matchToken(T_IDENTIFIER); if(token.type == T_PO) {matchToken(T_PO); formal_part(); matchToken(T_PF);}  matchToken(T_return);  matchToken(T_IDENTIFIER); break;
    default: printf("Subprogram specification error. Expecting T_procedure, T_function instead of: ");
             getMacro(token.type); exit(-1);
  }
}

void formal_part() {
  parameter_specification();
  while (token.type == T_PV) {
    scanToken();
    parameter_specification();
  }
}


void parameter_specification() {
  matchToken(T_IDENTIFIER);
  matchToken(T_2PT);
  switch (token.type) {
    case T_in: case T_out: scanToken();
  }
  matchToken(T_IDENTIFIER);
  if(token.type == T_ASSIGN) {
    scanToken();
    expression();
  }
}



void subprogram_body() {
  subprogram_specification();
  matchToken(T_is);
  while(token.type == T_type || token.type == T_IDENTIFIER) basic_declaration();
  matchToken(T_begin);
  sequence_statement();
  matchToken(T_end);
  if(token.type == T_IDENTIFIER) scanToken();
  matchToken(T_PV);

}




void sequence_statement() {
  statement();
  statements();
}


void statements() {
  switch (token.type) {
  case T_null: case T_IDENTIFIER: case T_exit: case T_goto:
  case T_procedure: case T_return: case T_requeue: case T_delay :
  case T_abort: case T_raise:
  case T_if: case T_case :  case T_declare:
  case T_loop: case T_while: case T_for: statement(); statements();
}
}

/* statement ::=  (simple_statement | compound_statement)
*/


void statement() {
  switch (token.type) {
    case T_null: case T_IDENTIFIER: case T_exit: case T_goto:
    case T_procedure: case T_return:  simple_statement(); break;
    case T_if: case T_case :  case T_declare:case T_begin:
    case T_loop: case T_while: case T_for: compound_statement(); break;
    default: printf("Invalide starting statement syntax: ");getMacro(token.type);exit(-1);

  }

}

/*
simple_statement ::= null_statement
   | assignment_statement | exit_statement
   | goto_statement | procedure_call_statement
   | return_statement | entry_call_statement
   | requeue_statement | delay_statement
   | abort_statement | raise_statement
   | code_statement
*/


void simple_statement() {
  switch (token.type) {
    case T_null: null_statement(); break;
    case T_IDENTIFIER: procedure_call_or_assign_statement(); break;
    case T_exit: exit_statement(); break;
    case T_goto: goto_statement(); break;
    case T_return: return_statement(); break;
    default:
    printf("Syntax error expecting one of the following tokens: T_null, T_IDENTIFIER, T_exit, T_goto, T_procedure, T_return, T_requeue, T_delay, T_abort, T_raise instead of :");
    getMacro(token.type); exit(-1);
  }
}
/*
compound_statement ::=
      if_statement
      | case_statement
      | loop_statement
      | block_statement
      | accept_statement
      | select_statement
*/

void compound_statement() {
  switch (token.type) {
    case T_if: if_statement(); break;
    case T_case : case_statement(); break;
    case T_loop: case T_while: case T_for: loop_statement(); break;
    case T_declare: case T_begin: block_statement(); break;
    default :printf("Syntax error expecting one of the following tokens: T_if, T_case, T_loop, T_while, T_for, T_declare instead of :");
            getMacro(token.type);
  }
}

void if_statement() {	
  AST ast1;	
  matchToken(T_if);
  expression();
  ast1=ast;
  ast=NULL;
  matchToken(T_then);
  list_inst_if=list_inst;
  list_inst=NULL;
  sequence_statement();
  while (token.type == T_elsif) {
    scanToken();
    expression();
    ast1=ast;
    ast=NULL;
    matchToken(T_then);
    sequence_statement();
  }
  if(token.type == T_else) {
    scanToken();
    sequence_statement();
  }

  instvalueType* p=creer_instruction_if(ast1, list_inst, NULL);
  list_inst=list_inst_if;
  list_inst_if=NULL;

  if(list_inst==NULL){
  		list_inst=(listinstvalueType*)malloc(sizeof(instvalueType));
  		list_inst->first=*p;
  }
  else{
  	inserer_inst_en_queue(list_inst,*p);
  }
  
  
  matchToken(T_end);
  matchToken(T_if);
  matchToken(T_PV);

}



void case_statement() {

  matchToken(T_case);
  expression();
  matchToken(T_is);
  case_statement_alt();
  case_statement_alts();
  matchToken(T_end);
  matchToken(T_case);
  matchToken(T_PV);

}
//case_statement_alternative ::= "when" discrete_choice_list "=>" sequence_of_statements
//discrete_choice_list ::= discrete_choice { "|" discrete_choice }

void case_statement_alt() {
  matchToken(T_when);
  switch (token.type) {
    case T_null: case T_NUMERIC: case T_STRING: case T_PO:
    case T_IDENTIFIER: simple_expression(); matchToken(T_EG); matchToken(T_SUP); sequence_statement(); break;
    case T_others: scanToken(); matchToken(T_EG); matchToken(T_SUP); sequence_statement();break;
    default :printf("Syntax error expecting one of the following tokens: T_null, T_NUMERIC, T_STRING, T_PO, T_IDENTIFIER, T_others instead of :");
            getMacro(token.type);
  }
}
// case_statement_alts ::= case_statement_alt case_statement_alts  | e
void case_statement_alts() {
  if(token.type == T_when) {
    case_statement_alt();
    case_statement_alts();
  }
}
/*
loop_statement ::= [ ( "while" expression ) | ( "for"
      id "in" [ "reverse" ] range ) ]
      "loop"
      sequence_of_statements
      "end" "loop" ";"
*/

void loop_statement() {
  int reverse=0;
  AST ast1=NULL;
  AST ast2=NULL;
  int type_instr=0;
  char nomVariable[30];
  listinstvalueType* list_inst_for, *tmp;
  instvalueType* p;

  if(token.type == T_while) {
    scanToken();
    expression();
    type_instr=2;
  }
  else if(token.type == T_for) {
  	type_instr=1;
    scanToken();
    matchToken(T_IDENTIFIER);
    strcpy(TS[n].name,nomToken);TS[n].value=0;
    strcpy(nomVariable,nomToken);
    n++;
	  p=creer_instruction_decl(0,nomVariable);
	  if(list_inst==NULL){
	  		list_inst=(listinstvalueType*)malloc(sizeof(instvalueType));
	  		list_inst->first=*p;
	  }
	  else{
	  	inserer_inst_en_queue(list_inst,*p);
	  }
    matchToken(T_in);

    if(token.type == T_reverse){ reverse=1; scanToken();}
    simple_expression();
    ast1=ast;
    ast=NULL;
    matchToken(T_PT);
    matchToken(T_PT);
    simple_expression();
    ast2=ast;
    ast=NULL;
  }
  matchToken(T_loop);
  list_inst_for=list_inst;
  list_inst=NULL;
  sequence_statement();
  tmp=list_inst;
  list_inst=list_inst_for;
  list_inst_for=tmp;
      if(type_instr==1){
	      	p=creer_instruction_for(indexVar(nomVariable,TS,n), ast1, ast2, reverse ,tmp);
		  if(list_inst==NULL){
		  		list_inst=(listinstvalueType*)malloc(sizeof(instvalueType));
		  		list_inst->first=*p;
		  }
		  else{
		  	inserer_inst_en_queue(list_inst,*p);
		  }
      }
      else if(type_instr==0){
      	
      }
  	  
	  
  

  matchToken(T_end);
  matchToken(T_loop);
  matchToken(T_PV);

}

/*
block_statement ::=
      [ "declare" declarative_part ]
      "begin"
      handled_sequence_of_statements
      "end"  ";"
*/





void block_statement() {

  if(token.type == T_declare) {
    scanToken();
    while(token.type == T_type || token.type == T_IDENTIFIER) basic_declaration();
  }
  matchToken(T_begin);
  sequence_statement();
  matchToken(T_end);

  matchToken(T_PV);

}




void basic_declaration() {
  switch (token.type) {
    case T_type: type_declaration(); break;
    case T_IDENTIFIER:object_number_declaration(); break;
    default: printf("Unknown declaration method \n"); exit(-1);
  }
}

void type_declaration() {
  matchToken(T_type);
  matchToken(T_IDENTIFIER);
  matchToken(T_is);
  switch (token.type) {
        case T_PO: enumeration_type_definition(); break;
        case T_mod: case T_range: integer_type_definition(); break;
        case T_digits: real_type_definition(); break;
        case T_array: array_type_definition(); break;
        case T_null: case T_record: record_type_definition(); break;
        default: printf("Expecting one of the following T_PO ,T_mod ,T_range ,T_digits ,T_array ,T_null, T_IDENTIFIER instead of: ");
                 getMacro(token.type); exit(-1);

  }
  matchToken(T_PV);

}



/*
  declarative_part ::= { ( basic_declarative_item | body ) }
  basic_declarative_item ::= basic_declaration | aspect_clause | use_clause
  body ::=  subprogram_body | package_body | task_body | protected_body
*/
/*

basic_declaration ::=
     type_declaration | subtype_declaration
   | object_declaration | object_number_declaration
   | subprogram_declaration | abstract_subprogram_declaration
   | null_procedure_declaration | expression_function_declaration
   | package_declaration | renaming_declaration
   | exception_declaration | generic_declaration
   | generic_instantiation

   full_type_declaration ::=
        type defining_identifier [known_discriminant_part] is type_definition
           [aspect_specification];


   type_definition ::=
        enumeration_type_definition | integer_type_definition
      | real_type_definition | array_type_definition
      | record_type_definition
      | derived_type_definition
*/

/*
enumeration_type_definition ::= (enumeration_literal_specification {, enumeration_literal_specification})
enumeration_literal_specification ::=  defining_identifier | defining_character_literal
defining_character_literal ::= character_literal
*/

void enumeration_type_definition() {

  matchToken(T_PO);
  enumeration_literal_specification();
  while(token.type == T_VIR) {
    scanToken();
    enumeration_literal_specification();
  }
  matchToken(T_PF);


}

void enumeration_literal_specification() {

  switch (token.type) {
    case T_IDENTIFIER: matchToken(T_IDENTIFIER); break;
    case T_CHAR:  matchToken(T_CHAR); break;
    default: printf("Expecting one of the following T_IDENTIFIER, T_CHAR instead of:");
             getMacro(token.type); exit(-1);

  }
}
/*

integer_type_definition ::= signed_integer_type_definition | modular_type_definition
signed_integer_type_definition ::= range T_num .. T_num
modular_type_definition ::= mod T_num
*/
void integer_type_definition() {
  switch (token.type) {
    case T_range:scanToken(); matchToken(T_NUMERIC); matchToken(T_PT); matchToken(T_PT); matchToken(T_NUMERIC); break;
    case T_mod:  scanToken(); matchToken(T_mod); expression(); break;
    default :printf("Expecting one of the following T_range, T_mod instead of:");
             getMacro(token.type); exit(-1);
  }
}
/*
floating_point_definition ::= digits num [real_range_specification]

real_range_specification ::= range num .. num
*/
void real_type_definition() {
  matchToken(T_digits);
  matchToken(T_NUMERIC);

  if(token.type == T_range){
    scanToken();
    matchToken(T_NUMERIC);
    matchToken(T_PT);
    matchToken(T_PT);
    matchToken(T_NUMERIC);
  }
}

//array_type_definition ::= unconstrained_array_definition | constrained_array_definition

// only 1-D array
void array_type_definition() {
  matchToken(T_array);
  matchToken(T_PO);

  matchToken(T_IDENTIFIER);
  if(token.type == T_range) {
    matchToken(T_range);
    if(token.type == T_INF) {
      matchToken(T_INF);
      matchToken(T_SUP);
    }
    else if(token.type == T_NUMERIC) {
      matchToken(T_NUMERIC);
      matchToken(T_PT);
      matchToken(T_PT);
      matchToken(T_NUMERIC);
    }
  }
  matchToken(T_PF);
  matchToken(T_of);
  matchToken(T_IDENTIFIER);


}

/*
record_definition ::= record component_list end record
                      | null record


component_list ::= component_item {component_item}
                  |  null;

component_item ::=  id : id [:= expression];

*/

void record_type_definition() {

  if(token.type == T_record) {
    matchToken(T_record);
    component_list();
    matchToken(T_end);
    matchToken(T_record);
    return;
  }
  matchToken(T_null);
  matchToken(T_record);
  matchToken(T_PV);

}

void component_list() {

  if(token.type == T_null){
    scanToken();
    matchToken(T_PV);
    return;
  }
  component_item();
  while(token.type == T_PV) {
    scanToken();
    if(token.type == T_end) break;
    component_item();
  }
}

void component_item() {

  matchToken(T_IDENTIFIER);
  matchToken(T_2PT);
  matchToken(T_IDENTIFIER);
  if(token.type == T_ASSIGN) {
    matchToken(T_ASSIGN);
    expression();
  }
}

void object_number_declaration() {
  matchToken(T_IDENTIFIER);
  strcpy(TS[n].name,nomToken);TS[n].value=0;
  instvalueType* p=creer_instruction_decl(TS[n].value,TS[n].name);
  if(list_inst==NULL){
  		list_inst=(listinstvalueType*)malloc(sizeof(instvalueType));
  		list_inst->first=*p;
  }
  else{
  	inserer_inst_en_queue(list_inst,*p);
  }
  n++;printf("DEEEEEECLLLLLL %d\n",n);
  matchToken(T_2PT);
  switch (token.type) {
    case T_IDENTIFIER: matchToken(T_IDENTIFIER); break;
    case T_constant : matchToken(T_constant); break;
    default: printf("Expecting one of the following: T_constant T_IDENTIFIER instead of");
             getMacro(token.type); exit(-1);
  }
  matchToken(T_ASSIGN);
  expression();
  instvalueType* affectation=creer_instruction_affectation(n-1, ast); 
  if(list_inst==NULL){
  		list_inst=(listinstvalueType*)malloc(sizeof(instvalueType));
  		list_inst->first=*affectation;
  }
  else{
  	inserer_inst_en_queue(list_inst,*affectation);
  }
  ast=NULL;
  matchToken(T_PV);
}





void null_statement() {
  matchToken(T_null);
  matchToken(T_PV);
}




void exit_statement() {
  matchToken(T_exit);

  if(token.type == T_IDENTIFIER) matchToken(T_IDENTIFIER);
  if(token.type == T_when)  {matchToken(T_when); expression();}
  matchToken(T_PV);
}

void goto_statement() {

  matchToken(T_goto);
  matchToken(T_IDENTIFIER);
  creer_instruction_goto(nomToken);
  instvalueType* inst=creer_instruction_goto(nomToken);
  if(list_inst==NULL){
		    list_inst=(listinstvalueType*)malloc(sizeof(listinstvalueType));
		    list_inst->first=*inst;
		    list_inst->next=NULL;
  }	
  else {
		    inserer_inst_en_queue(list_inst, *inst);
  }
  matchToken(T_PV);

}

//procedure_call_statement ::= id(params)
//params ::= param nextParam
//nextParam ::= e | , params
//param ::= [id =>] expression

void param() {

  if(token.type == T_IDENTIFIER) {
    matchToken(T_IDENTIFIER);
    if(token.type != T_EG) {
      return;

    }
    matchToken(T_EG);
    matchToken(T_SUP);
  }
  expression();

}

void params() {
  param();
  nextParam();
}
void nextParam() {
  if(token.type == T_VIR) {
     matchToken(T_VIR);
     params();
   }
}

void procedure_call_or_assign_statement() {
  instvalueType* inst;
  char nomVariable[30];
  matchToken(T_IDENTIFIER);
  strcpy(nomVariable,nomToken);printf("VARIABLE ===> %s\n",nomVariable);
  if(strcmp(nomToken,"Put")==0){
  		if(token.type == T_PO) { 	
		    matchToken(T_PO);

		    if(token.type == T_IDENTIFIER){
		    	params();

		    	inst=creer_instruction_print(indexVar(nomToken,TS,n));
		    	if(list_inst==NULL){
			    	list_inst=(listinstvalueType*)malloc(sizeof(listinstvalueType));
			    	list_inst->first=*inst;
			    	list_inst->next=NULL;
		   		 }	
		    	else {
		    		inserer_inst_en_queue(list_inst, *inst);
		   		 }
		    }
		    else{
		    	message=1;
		    	params();
		    	if(ast!=NULL){
		    		inst=creer_instruction_print_exp(ast);
		    		if(list_inst==NULL){
				    	list_inst=(listinstvalueType*)malloc(sizeof(listinstvalueType));
				    	list_inst->first=*inst;
				    	list_inst->next=NULL;
			   		}	
			    	else {
			    		inserer_inst_en_queue(list_inst, *inst);
			   		}
		    		ast=NULL;
		    	}
		    	message=0;	
		    }
		    
		    
		    matchToken(T_PF);
		    

	    }
	    else if(token.type == T_ASSIGN) {

	      scanToken();
	      expression();

	    }
  }
  else{
  	   if(token.type == T_PO) { 	
	    matchToken(T_PO);
	    params();
	    matchToken(T_PF);

	    }
	    else if(token.type == T_ASSIGN) {

	      scanToken();
	      expression();
	      inst=creer_instruction_affectation(indexVar(nomVariable,TS,n), ast);
		    if(list_inst==NULL){
		    	list_inst=(listinstvalueType*)malloc(sizeof(listinstvalueType));
		    	list_inst->first=*inst;
		    	list_inst->next=NULL;
		    }	
		    else {
		    	inserer_inst_en_queue(list_inst, *inst);
		    }	

	      ast=NULL;


	    }
  }
  
  matchToken(T_PV);

}

void return_statement() {

  matchToken(T_return);

  switch (token.type) {
    case T_PLUS: case T_MOINS: case T_abs : case T_not:
    case T_null: case T_NUMERIC: case T_STRING: case T_PO:
    case T_IDENTIFIER: expression();
  }
  matchToken(T_PV);
}



//expression ::= relation [(and | or | xor) relation]*
void expression() {
  relation();
  switch (token.type) {
    case T_and: case T_or: case T_xor: scanToken(); expression();
  }
}


void relation() {
  simple_expression();
  switch (token.type) {
    case T_EG: case T_DIV: case T_SUP: case T_DIFF:
    case T_SUPEG: case T_INF: case T_INFEG:  scanToken(); simple_expression(); break;
    case T_not: case T_in: scanToken(); matchToken(T_NUMERIC); matchToken(T_PT); matchToken(T_PT); matchToken(T_NUMERIC);
  }
}

//simple_expression ::= [ ( "+" | "-" ) ] term { ( "+" | "-" | "&" ) term }*

void simple_expression() {
  instvalueType* inst;
  switch (token.type) {
    case T_PLUS:  case T_MOINS: scanToken();
    case T_NUMERIC:  if(ast==NULL)ast=creer_feuille_nombre(atof(token.val.stringValue), Int);
                     else ast->noeud.op.expression_droite=creer_feuille_nombre(atof(token.val.stringValue), Int);
                     printf("========>%s\n",token.val.stringValue);message=2;
    case T_null: 
    case T_STRING: if(message==1){inst=creer_instruction_message(token.val.stringValue);
				    if(list_inst==NULL){
						    	list_inst=(listinstvalueType*)malloc(sizeof(listinstvalueType));
						    	list_inst->first=*inst;
						    	list_inst->next=NULL;
						    }	
						    else {
						    	inserer_inst_en_queue(list_inst, *inst);
						    }	}
    case T_PO:
    case T_IDENTIFIER:term(); term_cat(); break;
    default: printf("Expecting : T_NULL, T_NUMERIC, T_STRING, T_PO, T_IDENTIFIER instead of ");getMacro(token.type); exit(-1);
  }
}
//
//term_cat ::= e | (+|-|&) term term_cat
//follow of term:  = | /=  | <   | <= | > | >=
//

void term_cat() {
    switch (token.type) {
      case T_PLUS:  ast=creer_noeud_operation(atoi(token.val.stringValue), ast, NULL, plus); scanToken();term(); term_cat(); break;
      case T_MOINS: ast=creer_noeud_operation(atoi(token.val.stringValue), ast, NULL, minus); scanToken(); term(); term_cat();break;
      case T_CONCAT:   term(); term_cat();
      /*case T_EG: case T_DIV: case T_SUP:
      case T_SUPEG: case T_INF: case T_INFEG:   simple_expression();break;
      default: printf("Expecting one of the following: T_PLUS, T_MOINS, T_CONCAT, T_DIV, T_SUP, T_SUPEG, T_INF, T_INFEG instead of: " );
               getMacro();
      */
    }
}
//term ::= factor {multiplying_operator factor}
void term() {
  factor();
  switch (token.type) {
    case T_MULT: case T_DIV: case T_mod: case T_rem: scanToken();  term();
  }
}

void primary() {

  switch (token.type) {
    case T_NUMERIC: if(ast==NULL)ast=creer_feuille_nombre(atof(token.val.stringValue), Int);
                    else ast->noeud.op.expression_droite=creer_feuille_nombre(atof(token.val.stringValue), Int);
    case T_STRING:
    case T_IDENTIFIER: scanToken(); break;
    case T_PO:  scanToken(); expression(); matchToken(T_PF);   break;
    default: printf("Expecting :T_NULL, T_NUMERIC, T_STRING, T_PO, T_IDENTIFIER instead of ");getMacro(token.type); exit(-1);
  }

}


void factor() {
  switch (token.type) {
    case T_NUMERIC: if(ast==NULL)ast=creer_feuille_nombre(atof(token.val.stringValue), Int);
                    else ast->noeud.op.expression_droite=creer_feuille_nombre(atof(token.val.stringValue), Int);
    case T_null:                 
    case T_STRING: case T_PO:
    case T_IDENTIFIER: primary(); if(token.type == T_POWER) {primary();}
                      break;
    case T_abs:
    case T_not:    primary(); break;
    default: printf("Expecting : T_abs, T_not, T_NULL, T_NUMERIC, T_STRING, T_PO, T_IDENTIFIER instead of: " );
              getMacro(token.type); exit(-1);
  }
}





int main(int argc, char const *argv[]) {

  fp = fopen("dummy.txt" , "r+");
  file = fopen("generer.txt" , "w+");
  str = calloc(1024 , sizeof(char));
  list_inst=NULL;
  scanToken();
  program();
  generer_pseudo_code_list_inst(list_inst, file);
  fclose(file);


  return 0;
}