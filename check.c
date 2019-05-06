#include "global.h"
#include "check.h"

void SYS(int returnCode, char* errorMessage) {
	if(returnCode == -1 ){
		perror(errorMessage);
		perror("\n");
		exit(1);
	}
}

void SYSN(void* returnVal, char* errorMessage) {
	if(returnVal == NULL) {
		perror(errorMessage);
		perror("\n");
		exit(1);
	}
}