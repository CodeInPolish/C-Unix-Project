#include "global.h"
#include "sockets.h"

int main(int argv, char** argc){
	int socketFd = setupClientSocket(8080);
	printf("Socket id: %d \n", socketFd);
	while(1){
	}
	closeSocket(socketFd);
	exit(0);
}