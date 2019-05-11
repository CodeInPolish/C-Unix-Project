#include "global.h"
#include "ipc.h"

int main(int argc, char const *argv[]){
	

	if(argc==2){

		printf("%d\n", atoi(argv[1]));
		int numProgramme=char2int(argv[1]);
		down(0);
		Ressource* ptr_mem_partagee = getMemory();
		Ressource program = ptr_mem_partagee[numProgramme];
		printf("%d\n",  program.number);
		printf("%s\n",  program.name);
		printf("%d\n",  program.compile);
		printf("%d\n",  program.execNumber);
		printf("%d\n",  program.execTime);
		up(0);


	}else{
		perror("error call => gstat n");

	}
}