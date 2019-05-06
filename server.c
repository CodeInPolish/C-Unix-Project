#include "global.h"
#include "sockets.h"

int main(int argv, char** argc){

	if(argv != 2) {
		printf("One parameter needed. Got %d\n", argv-1);
		exit(1);
	}
	int port = (int) strtol(argc[1], NULL, 10);
	printf("Starting server.\n");
	int socketFd = setupServerSocket(port);
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
