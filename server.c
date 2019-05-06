#include "global.h"
#include "sockets.h"

int main(int argv, char** argc){

	if(argv != 2) {
		printf("One parameter needed. Got %d\n", argv-1);
		exit(1);
	}
	
	printf("Starting server.\n");
	int socketFd = setupServerSocket(8080);
	printf("Server started.\n");
	int newConn;
	while(1){
		if((newConn = accept(socketFd, NULL, NULL)) != -1){
			printf("Got a connection!\n");
		}
	}
	printf("Closing server.\n");
	close(socketFd);
	exit(0);
}
