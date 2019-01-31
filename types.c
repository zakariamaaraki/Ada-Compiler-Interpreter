#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pseudoCode.h"

extern namevalue TS[100];
extern int n;


instvalueType* creer_instruction_print(int rangvar){

	instvalueType* inst = (instvalueType *) malloc (sizeof(instvalueType));

	inst->typeinst = PrintIdf;
	inst->node.printnode.rangvar = rangvar;
	
	return inst;
}

instvalueType* creer_instruction_decl(double value, char* name){
	instvalueType* inst = (instvalueType *) malloc (sizeof(instvalueType));

    inst->typeinst=DECL;
	inst->node.declnode.value = value;
	strcpy(inst->node.declnode.name,name);
	
	return inst;
}

instvalueType* creer_instruction_message(char* message){

	instvalueType* inst = (instvalueType*) malloc (sizeof(instvalueType));
    char messageTmp[30];
    memset(messageTmp,'\0',30);
    int i=1;
    for(i=1;i<strlen(message)-1;i++){
    	messageTmp[i-1]=message[i];
    }

	inst->typeinst = Message;
	strcpy(inst->node.messagenode.message,messageTmp);

	return inst;
}

instvalueType* creer_instruction_print_exp(AST ast){

	instvalueType* inst = (instvalueType*) malloc (sizeof(instvalueType));
	inst->typeinst = PrintExp;
	inst->node.printExpnode.exp=ast;

	return inst;
}

instvalueType* creer_instruction_affectation(int rangvar, AST past){

	instvalueType* inst = (instvalueType*) malloc (sizeof(instvalueType));

	inst->typeinst = AssignArith;
	inst->node.assignnode.rangvar = rangvar;
	inst->node.assignnode.right = past;

   //semantique----------------------------
	   if(past->typeexp==_IDF){
			if(past->typename!=TS[rangvar].type){				
						fprintf(stderr, "the variable %s and %s do not have the same type\n",TS[rangvar].name,past->noeud.idf);exit(-1);			  
			}
	    }	
    //------------------------------------

	return inst;
}

instvalueType* creer_instruction_if(AST past, listinstvalueType * plistthen, listinstvalueType * plistelse){

	instvalueType* inst = (instvalueType *) malloc (sizeof(instvalueType));
	inst->typeinst = ((plistelse != NULL)?IfThenElseArith:IfThenArith);
	inst->node.ifnode.right = past;
	inst->node.ifnode.thenlinst = plistthen;
	inst->node.ifnode.elselinst = plistelse;

	return inst;
}

instvalueType* creer_instruction_for(int rangvar, AST borneinf, AST bornesup, int reverse, listinstvalueType* pplistfor){

	instvalueType* inst = (instvalueType *) malloc (sizeof(instvalueType));

	inst->typeinst = For;
	inst->node.fornode.rangvar = rangvar;
	inst->node.fornode.reverse= reverse;
	inst->node.fornode.borneinf = borneinf;
	inst->node.fornode.bornesup = bornesup;
	inst->node.fornode.forbodylinst = pplistfor;

	return inst;

}


instvalueType* creer_instruction_while(AST cond, listinstvalueType* pplistwhile){

	instvalueType* inst = (instvalueType *) malloc (sizeof(instvalueType));

	inst->typeinst = While;
	inst->node.whilenode.cond = cond;
	inst->node.whilenode.whilebodylinst = pplistwhile;

	return inst;

}

instvalueType* creer_instruction_loop(listinstvalueType* pplistloop){

	instvalueType* inst = (instvalueType *) malloc (sizeof(instvalueType));

	inst->typeinst = Loop;
	inst->node.loopnode.loopbodylinst = pplistloop;

}

instvalueType* creer_instruction_exit(AST cond){

	instvalueType* inst = (instvalueType *) malloc (sizeof(instvalueType));
    inst->node.exitnode.cond = cond;
	inst->typeinst = Exit;
}

instvalueType* creer_instruction_goto(char* label){

	instvalueType* inst = (instvalueType *) malloc (sizeof(instvalueType));

	inst->typeinst = Goto;
	strcpy(inst->node.gotonode.label,label);

	return inst;

}

instvalueType* creer_instruction_label(char* label){

	instvalueType* inst = (instvalueType *) malloc (sizeof(instvalueType));

	inst->typeinst = Label;
	strcpy(inst->node.labelnode.label,label);

	return inst;

}

void inserer_inst_en_queue(listinstvalueType * pp, instvalueType p){
  listinstvalueType* liste = (listinstvalueType*) malloc(sizeof(listinstvalueType));
  liste->first = p;
  liste->next = NULL;

  if (pp->next == NULL) {
    pp->next = liste;
  }else{
    listinstvalueType * pliste = pp;
    
    while(pliste->next != NULL) {
      pliste = pliste->next;
    }
    
    pliste->next = liste;
  }
}

void generer_pseudo_code_inst(instvalueType p, FILE* file){
  static int forlabel_index = 0;
  static int whilelabel_index = 0;
  static int looplabel_index = 0;
  static int iflabel_index=0;
  static int exitlabel_index=0;
  int i;

  switch(p.typeinst){
  	case PrintIdf :
  	    fprintf(file,"----------------------\nAffichage de %s\n----------------------\nLOAD %s\n", name(p.node.printnode.rangvar,TS),name(p.node.printnode.rangvar,TS));
	    fprintf(file,"PRNT ");
	    fprintf(file,"\n----------------------\n\n");
	    break;
	case Message :
		fprintf(file,"MESSAGE %s\n\n",p.node.messagenode.message);
		break; 
	case Exit : 
		generer_pseudo_code_ast(p.node.exitnode.cond,file);	 
		fprintf(file,"PUSH 0\n");
		fprintf(file,"JNE exit%d\n",exitlabel_index);
		fprintf(file,"JMP endexit%d\n",exitlabel_index);
		fprintf(file,"LABEL exit%d\n",exitlabel_index);
		fprintf(file,"JMP endloop%d\n",looplabel_index);
  		fprintf(file,"LABEL endexit%d\n\n",exitlabel_index);
  		break;
	case PrintExp:
		fprintf(file,"\n----------------------\nAffichage d'expression\n----------------------\n");
		generer_pseudo_code_ast(p.node.printExpnode.exp,file);
		fprintf(file,"PRNT\n----------------------\n\n");
		break;	
	case AssignArith :
		fprintf(file,"----------------------\n Affectation \n----------------------\n");
	    generer_pseudo_code_ast(p.node.assignnode.right,file);
	    fprintf(file,"STORE ");
	    fprintf(file,"%s ", name(p.node.assignnode.rangvar,TS));
	    fprintf(file,"\n----------------------\n\n");
	    break;    
	case IfThenArith :
	    iflabel_index++;
	    fprintf(file,"----------------------\n Cond %d : if \n----------------------\n",iflabel_index);
	    generer_pseudo_code_ast(p.node.ifnode.right,file);
	    fprintf(file,"PUSH 0\n");
	    fprintf(file,"JNE if%d\n",iflabel_index);
	    fprintf(file,"JMP ifFalse%d\n",iflabel_index);
	    fprintf(file,"LABEL if%d\n",iflabel_index);
	    generer_pseudo_code_list_inst(p.node.ifnode.thenlinst, file);
	    fprintf(file,"LABEL ifFalse%d\n----------------------\n\n",iflabel_index);
	    break;
	case IfThenElseArith : 
	    iflabel_index++;
	    fprintf(file,"----------------------\n Cond %d : if \n----------------------\n",iflabel_index);
	    generer_pseudo_code_ast(p.node.ifnode.right,file);
	    fprintf(file,"PUSH 0\n");
	    fprintf(file,"JNE if%d\n",iflabel_index);
	    fprintf(file,"JMP ifFalse%d\n",iflabel_index);
	    fprintf(file,"LABEL if%d\n",iflabel_index);
	    generer_pseudo_code_list_inst(p.node.ifnode.thenlinst, file);
	    fprintf(file,"JMP ifTrue%d\n",iflabel_index);
	    fprintf(file,"LABEL ifFalse%d\n----------------------\n Cond %d : else \n----------------------\n",iflabel_index,iflabel_index);
	    generer_pseudo_code_list_inst(p.node.ifnode.elselinst, file);
	    fprintf(file,"LABEL ifTrue%d\n----------------------\n\n",iflabel_index);
	    break;    
	case For :
	   forlabel_index++;
	   fprintf(file,"----------------------\n Boucle for %d \n----------------------\n",forlabel_index);
	   if(p.node.fornode.reverse==0)generer_pseudo_code_ast(p.node.fornode.borneinf,file);
	   else generer_pseudo_code_ast(p.node.fornode.bornesup,file);

	   fprintf(file,"STORE ");
	   fprintf(file,"%s ",name(p.node.fornode.rangvar,TS));
	   fprintf(file,"\n");
	   fprintf(file,"LABEL for%d\n",forlabel_index);
	   if(p.node.fornode.reverse==0){
	   	generer_pseudo_code_ast(p.node.fornode.bornesup,file);
	   	fprintf(file,"LOAD ");
	   fprintf(file,"%s ",name(p.node.fornode.rangvar,TS));
	   fprintf(file,"\n");
	   }
	   else{
	   	fprintf(file,"LOAD ");
	   fprintf(file,"%s ",name(p.node.fornode.rangvar,TS));
	   fprintf(file,"\n");
	   		generer_pseudo_code_ast(p.node.fornode.borneinf,file);
	   } 
	   
	   fprintf(file,"JG endfor%d\n",forlabel_index);

	   fprintf(file,"PUSH ");
	   fprintf(file,"\n");
	   fprintf(file,"LOAD ");
	   fprintf(file,"%s ",name(p.node.fornode.rangvar,TS));
	   fprintf(file,"\n");
	   

	   generer_pseudo_code_list_inst(p.node.fornode.forbodylinst, file);

	   fprintf(file,"PUSH 1\n");
	   fprintf(file,"LOAD ");
	   fprintf(file,"%s ",name(p.node.fornode.rangvar,TS));
	   fprintf(file,"\n");
	   if(p.node.fornode.reverse==0)fprintf(file,"ADD ");
	   else fprintf(file,"SUB ");
	   fprintf(file,"\n");
	   fprintf(file,"STORE ");
	   fprintf(file,"%s ",name(p.node.fornode.rangvar,TS));
	   fprintf(file,"\n");
       
       fprintf(file,"JMP for%d\n",forlabel_index);	
	   fprintf(file,"LABEL endfor%d\n----------------------\n\n",forlabel_index);
	   break;       
	case Loop:  
	   looplabel_index++;
	   fprintf(file,"LABEL loop%d \n",looplabel_index);
	   generer_pseudo_code_list_inst(p.node.loopnode.loopbodylinst, file);
       fprintf(file,"JMP loop%d \n",looplabel_index); 
       fprintf(file,"LABEL endloop%d \n",looplabel_index);   
	   break; 
	case While:
		whilelabel_index++;
		fprintf(file,"----------------------\n Boucle While %d \n",whilelabel_index);
		fprintf(file,"----------------------\nLABEL Debutwhile%d\n",whilelabel_index);
	    generer_pseudo_code_ast(p.node.whilenode.cond,file);
	    fprintf(file,"PUSH 0.000000\n");
	    fprintf(file,"JNE whileTrue%d\n",whilelabel_index);
		fprintf(file,"JMP endwhile%d\n",whilelabel_index);
	    fprintf(file,"LABEL whileTrue%d\n",whilelabel_index);
	    generer_pseudo_code_list_inst(p.node.whilenode.whilebodylinst, file);
	    fprintf(file,"JMP Debutwhile%d\n",whilelabel_index);	
	    fprintf(file,"LABEL endwhile%d\n----------------------\n\n",whilelabel_index);
		break;
	case Case:	
		/*caselabel_index++;
		for(i=0;i<p.node.casenode.nbCases;i++){
			fprintf(file,"LOAD %s ",name(p.node.casenode.rangvar,TS));
			fprintf(file,"PUSH %lf\n",p.node.casenode.c[i].val);
			fprintf(file,"JNE caselabel%d.%lf\n",caselabel_index,p.node.casenode.c[i].val);
			generer_pseudo_code_list_inst(p.node.casenode.c[i].caselinst, file);
			fprintf(file,"LABEL caselabel%d.%lf\n",caselabel_index,p.node.casenode.c[i].val);
		}*/
		break;     
	case Goto:
		fprintf(file,"----------------------\nGoto\n----------------------\nJMP %s\n----------------------\n\n",p.node.gotonode.label);
		break;	
	case Label:
		fprintf(file,"LABEL %s\n",p.node.labelnode.label);
		break;	
	case DECL:	
		fprintf(file,"----------------------\nDeclaration de %s\n----------------------\nDATA %s %lf\n----------------------\n\n",p.node.declnode.name,p.node.declnode.name,p.node.declnode.value);
		break;    
  }
  
}

void generer_pseudo_code_list_inst(listinstvalueType* p, FILE* file){
  if(p != NULL){
   generer_pseudo_code_inst(p->first, file);    
   generer_pseudo_code_list_inst(p->next, file);
  } 
}
