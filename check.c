#include "global.h"
#include "check.h"

int SYS(int returnCode, char* errorMessage) {
	if(returnCode == -1 ){
		perror(errorMessage);
		perror("\n");
		exit(1);
	}
	return returnCode;
}

void* SYSN(void* returnVal, char* errorMessage) {
	if(returnVal == NULL) {
		perror(errorMessage);
		exit(1);
	}
	return returnVal;
}