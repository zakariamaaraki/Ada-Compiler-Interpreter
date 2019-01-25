
#include <stdio.h>
#include <stdlib.h>
#include "pseudoCode.h"

int main(int argc, char** argv){
	if(argc!=2){
		fprintf(stderr,"Vous devez entrer deux arguments ! \n");
		exit(-1);
	}
	

	loadPseudoCode(argv[1]);

	return 0;
	
}