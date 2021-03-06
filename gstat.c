#include "global.h"
#include "ipc.h"

int main(int argc, char const *argv[]){

	if(argc==2){
		int numProgramme=char2int(argv[1]);
		down(0);
		Ressource* ptr_mem_partagee = getMemory();
		Ressource program = ptr_mem_partagee[numProgramme];
		printf("number:%d\n",  program.number);
		printf("name:%s\n",  program.name);
		printf("compile:%d\n",  program.compile);
		if(program.compile) {
			printf("execNumber:%d\n",  program.execNumber);
			printf("execTime:%ld\n",  program.execTime);
		} else {
			printf("execNumber:%d\n",  0);
			printf("execTime:%d\n",  0);
		}
		
		up(0);
	}else{
		perror("error call => gstat n");
	}
}