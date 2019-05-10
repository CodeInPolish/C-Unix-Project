#include "global.h"
#include "ipc.h"

int main(int argc, char const *argv[]){
	

	if(argc==2){

		printf("%d\n", atoi(argv[1]));
		int numProgramme=char2int(argv[1]);
		down(0);
		getMemory();
		printf("%d\n",  ptr_mem_partagee[numProgramme].num_programme);
		printf("%s\n",  ptr_mem_partagee[numProgramme].nom);
		printf("%d\n",  ptr_mem_partagee[numProgramme].compile);
		printf("%d\n",  ptr_mem_partagee[numProgramme].nombre_exec);
		printf("%d\n",  ptr_mem_partagee[numProgramme].temps_exec);
		up(0);


	}else{
		perror("error call => gstat n");

	}
}