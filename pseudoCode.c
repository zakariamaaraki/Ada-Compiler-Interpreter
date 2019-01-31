
#include "pseudoCode.h"
#include "pile.h"
#include <string.h>

Pile VM;
namevalue TS[1000];
int taille;
pseudocode pc;
pseudocode p;

void loadPseudoCode(char* nameFile){
	FILE* file=fopen(nameFile,"r");
	if(file==NULL){
		fprintf(stderr,"Erreur d'ouverture du fichier %s !\n",nameFile);
		exit(-1);
	}

	char codop[30];
	char var[30];
	double value;

	initialiser_VM();
	initialiser_TS();

	pc=NULL;
	while(fscanf(file,"%s",codop)>0){
		if(pc==NULL){
			pc=(pseudocode)malloc(sizeof(struct pseudocodenode));
			p=pc;
		}	
	    else{
	    	pc->next=(pseudocode)malloc(sizeof(struct pseudocodenode));
	    	pc=pc->next;
	    }	
		if(strcmp(codop,"PRNT")==0){
			(pc->first).codop=PRNT;
		}
		else if(strcmp(codop,"STORE")==0){
			(pc->first).codop=STORE;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.var,var);			
		}
		else if(strcmp(codop,"ADD")==0){
			(pc->first).codop=ADD;
		}
		else if(strcmp(codop,"SUB")==0){
			(pc->first).codop=SUB;
		}
		else if(strcmp(codop,"MULT")==0){
			(pc->first).codop=MULT;
		}
		else if(strcmp(codop,"DIV")==0){
			(pc->first).codop=DIV;
		}
		else if(strcmp(codop,"DUPL")==0){
			(pc->first).codop=DUPL;
		}
		else if(strcmp(codop,"SWAP")==0){
			(pc->first).codop=SWAP;
		}
		else if(strcmp(codop,"LOAD")==0){
			(pc->first).codop=LOAD;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.var,var);
		}
		else if(strcmp(codop,"PUSH")==0){
			(pc->first).codop=PUSH;
			fscanf(file,"%lf",&value);
			(pc->first).param._const=value;
		}
		else if(strcmp(codop,"LABEL")==0){
			(pc->first).codop=LABEL;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.label_name,var);
		}
		else if(strcmp(codop,"DATA")==0){
			(pc->first).codop=DATA;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.nv.name,var);
			fscanf(file,"%lf",&value);
			(pc->first).param.nv.value=value;
		}
		else if(strcmp(codop,"JMP")==0){
			(pc->first).codop=JMP;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.label_name,var);
		}
		else if(strcmp(codop,"JG")==0){
			(pc->first).codop=JG;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.label_name,var);
		}
		else if(strcmp(codop,"JGE")==0){
			(pc->first).codop=JGE;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.label_name,var);
		}
		else if(strcmp(codop,"JNE")==0){
			(pc->first).codop=JNE;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.label_name,var);
		}
		else if(strcmp(codop,"SCAN")==0){
			(pc->first).codop=SCAN;
		}
		else if(strcmp(codop,"MESSAGE")==0){
			(pc->first).codop=MESSAGE;
			fgets(var,100,file);
			int i;
			for(i=0;i<100;i++){
				if(var[i]=='\n')var[i]=' ';
			}
			strcpy((pc->first).param.var,var);
		}
		pc->next=NULL;
	}
	fclose(file);
	pc=p;
	interpreter_pseudo_code_list_inst();
}

double value(char* name){
	int i;
	for(i=0;i<taille;i++){
		if(strcmp(TS[i].name,name)==0)return TS[i].value;
	}
	return 0;
}

char* name(int rang, namevalue* TS){
	return TS[rang].name;
}

double value2(int rang, namevalue* TS){
	return TS[rang].value;
}

int indexVar(char* name, namevalue* TS, int n){
	int i;
	for(i=0;i<n;i++){
		if(strcmp(TS[i].name,name)==0)return i;
	}
 	return -1;
}

void store(char* name, double value){
	int i;
	for(i=0;i<taille;i++){
		if(strcmp(TS[i].name,name)==0)TS[i].value=value;
	}
}

void data(char* name, double value){
	strcpy(TS[taille].name,name);
	TS[taille++].value=value;
}

void initialiser_TS(){
	taille=0;
}

void initialiser_VM(){
	VM.sommet=-1;
}

pseudoinstruction rechercher_instruction_au_label(char* label){
	pseudocode pp=p;
	while(pp!=NULL){
		if((pp->first).codop==LABEL){
			if(strcmp((pp->first).param.label_name,label)==0){
				pc=pp;
				break;
			}	
		}
		pp=pp->next;
	}
}

void interpreter_pseudo_code_list_inst(){
	if (pc != NULL) {
		// interpretation de la première l’instruction
		interpreter_pseudo_code_inst( pc->first ) ;
		// appel récursif sur la suite de pseudocode
		pc=pc->next;
		interpreter_pseudo_code_list_inst() ;
	}
}

void interpreter_pseudo_code_inst(pseudoinstruction pci) {
	double op1,op2;
	switch(pci.codop){
		case PRNT :
			op1=depiler(&VM);
			printf("%lf \n",op1);
			break;
		case STORE : 
			op1=depiler(&VM);
			store(pci.param.var,op1);
			break;
		case ADD :
			op1 = depiler(&VM) ;
			op2 = depiler(&VM) ;
			empiler(&VM,op1 + op2) ;
			break ;
		case SUB :
			op1 = depiler(&VM) ;
			op2 = depiler(&VM) ;
			empiler(&VM,op1 - op2) ;
			break ;	
		case MULT:
			op1 = depiler(&VM) ;
			op2 = depiler(&VM) ;
			empiler(&VM,op1*op2) ;
			break ;	
		case DIV :
			op1 = depiler(&VM) ;
			op2 = depiler(&VM) ;
			empiler(&VM,op1/op2) ;
			break ;
		case DUPL :
			empiler(&VM,tetepile(VM)) ;
			break ;
		case JMP :
			interpreter_pseudo_code_inst(rechercher_instruction_au_label(pci.param.label_name));
			break ;
		case JNE :
			op1 = depiler(&VM);
			op2 = depiler(&VM);
			if(op1!=op2)interpreter_pseudo_code_inst(rechercher_instruction_au_label(pci.param.label_name));
			break ;	
		case JG :
			op1=depiler(&VM);
			op2=depiler(&VM);
			if(op1>op2)interpreter_pseudo_code_inst(rechercher_instruction_au_label(pci.param.label_name));
			break ;	
		case JGE :
			op1=depiler(&VM);
			op2=depiler(&VM);
			if(op1>=op2)interpreter_pseudo_code_inst(rechercher_instruction_au_label(pci.param.label_name));
			break ;	
		case LABEL :
			//rien faire;
			break;	
		case LOAD :
			empiler(&VM,value(pci.param.var));
			break ;
		case SWAP :
			op1 = depiler(&VM) ;
			op2 = depiler(&VM) ;
			empiler(&VM,op1) ;
			empiler(&VM,op2) ;
			break ;
		case PUSH : 
			empiler(&VM,pci.param._const);
		    break;	
		case DATA :		
			data(pci.param.nv.name,pci.param.nv.value);
			break;	
		case SCAN :
		    scanf("%lf",&op1);
		    empiler(&VM,op1);
		    break;
		case MESSAGE : 
		 	printf("%s",pci.param.var);
		 	break;    	
		
	}
}
