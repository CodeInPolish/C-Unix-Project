#include "global.h"
#include "sockets.h"

int main(int argv, char** argc){
	int socketFd = setupClientSocket(8080);
	printf("Socket id: %d \n", socketFd);
	char buffer[1];
	while(1){
		read(socketFd, &buffer, sizeof(buffer));
		printf("%c\n", buffer[0]);
	}
	closeSocket(socketFd);
	exit(0);
}