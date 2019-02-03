
#include "pseudoCode.h"
#include "pile.h"
#include <string.h>

Pile VM;
namevalue TS[1000];
int taille;
pseudocode pc;
pseudocode p;

array Array[100];
int nbArray=0;


void loadPseudoCode(char* nameFile){
	FILE* file=fopen(nameFile,"r");
	if(file==NULL){
		fprintf(stderr,"Erreur d'ouverture du fichier %s !\n",nameFile);
		exit(-1);
	}

	char codop[30];
	char var[30];
	double value;
	int val;

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
			fscanf(file,"%lf",&value);
			(pc->first).param.nv.type=(int)value;
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
		else if(strcmp(codop,"ARRAY")==0){
			(pc->first).codop=ARRAY;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.Array.name,var);
			fscanf(file,"%d",&val);
			(pc->first).param.Array.type=val;
			fscanf(file,"%d",&val);
			(pc->first).param.Array.bInf=val;
			fscanf(file,"%d",&val);
			(pc->first).param.Array.bSup=val;
		}
		else if(strcmp(codop,"PRINTARRAY")==0){
			(pc->first).codop=PRINTARRAY;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.Array.name,var);
		}
		else if(strcmp(codop,"AFFARRAY")==0){
			(pc->first).codop=AFFARRAY;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.Array.name,var);
		}
		else if(strcmp(codop,"LOADARRAY")==0){
			(pc->first).codop=LOADARRAY;
			fscanf(file,"%s",var);
			strcpy((pc->first).param.Array.name,var);
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

void data(char* name, double value, int type){
	strcpy(TS[taille].name,name);
	TS[taille].type=type;
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
	static int isInteger=0;
	static int isBoolean=0;
	int i,j; 
	switch(pci.codop){
		case PRNT :
			op1=depiler(&VM);
			if(isInteger==0 && isBoolean==0)printf("%lf \n",op1);
			else if(isInteger==1){printf("%d \n",(int)op1); isInteger=0;}
			else if(isBoolean==1){if(op1==0)printf("False \n");else printf("True\n"); isBoolean=0;}
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
			if(op2==0){
				printf("Exception : 'ArithmeticException' \n");exit(-1);
			}
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
			if(TS[indexVar(pci.param.var,TS,taille)].type==0){isInteger=1; isBoolean=0;}
			else if(TS[indexVar(pci.param.var,TS,taille)].type==3){isInteger=0; isBoolean=1;}
			else {isInteger=0; isBoolean=0;}
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
			data(pci.param.nv.name,pci.param.nv.value,pci.param.nv.type);
			break;	
		case SCAN :
		    scanf("%lf",&op1);
		    empiler(&VM,op1);
		    break;
		case MESSAGE : 
		 	printf("%s",pci.param.var);
		 	break;    
		case ARRAY :
		    i=(pci.param.Array.bSup-pci.param.Array.bInf+1);
		    //printf("%d\n",i);
			strcpy(Array[nbArray].name,pci.param.Array.name);
			Array[nbArray].type=pci.param.Array.type;
			Array[nbArray].T=(double*)malloc(sizeof(double)*(i));
			Array[nbArray].length=(int)(pci.param.Array.bSup-pci.param.Array.bInf)+1;
			j=0;
			for(i=pci.param.Array.bInf;i<=pci.param.Array.bSup;i++){
				Array[nbArray].T[j]=i;
				j++;
			}
		    nbArray++;
		    break;
		case PRINTARRAY :
			strcpy(Array[nbArray].name,pci.param.Array.name);
			for(i=0;i<nbArray;i++){
				if(strcmp(Array[i].name,pci.param.Array.name)==0){
					if(Array[i].type==0){
						printf("[ ");
						for(j=0;j<Array[i].length-1;j++){
							printf("%d, ",(int)Array[i].T[j]);
						}
						printf("%d ]\n",(int)Array[i].T[j]);
					}
					else if(Array[i].type==1 || Array[i].type==2){
						printf("[ ");
						for(j=0;j<Array[i].length-1;j++){
							printf("%lf, ",Array[i].T[j]);
						}
						printf("%lf ]\n",Array[i].T[j]);
					}
					else if(Array[i].type==3){
						printf("[ ");
						for(j=0;j<Array[i].length-1;j++){
							if(Array[i].T[j]!=0)printf("True, ");
							else printf("False, ");
						}
						if(Array[i].T[j]!=0)printf("True ]\n");
						else printf("False ]\n");
					}
				}
			}
		    break;  
		case AFFARRAY :
			for(i=0;i<nbArray;i++){
				if(strcmp(Array[i].name,pci.param.Array.name)==0){
					op1=depiler(&VM);
					if(op1>=Array[i].length || op1<0){
						printf("Exception : 'ArrayOutOfBoundException' => %s \n",Array[i].name);exit(-1);
					}
					else {
						op2=depiler(&VM);
						Array[i].T[(int)op1]=op2;
					}
				}	
			}
			break;	
		case LOADARRAY :
			for(i=0;i<nbArray;i++){
				if(strcmp(Array[i].name,pci.param.Array.name)==0){
					op1=depiler(&VM);
					if(op1<0 || op1>Array[i].length){
						printf("Exception : 'ArrayOutOfBoundException' => %s \n",Array[i].name);exit(-1);
					}
					empiler(&VM,Array[i].T[(int)op1]);
				}
			}	
			break;	      
	}
}
