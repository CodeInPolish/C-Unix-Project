#include "global.h"
#include "sockets.h"

int main(int argv, char** argc){

	if(argv != 2) {
		printf("1 parameter needed. Got %d\n", argv-1);
		exit(1);
	}
	int port = char2int(argc[1]);
	printf("Starting server.\n");
	int socketFd = setupServerSocket(port);
	printf("Server started.\n");
	int newConn;
	int childPid;
	while(1){
		if((newConn = accept(socketFd, NULL, NULL)) != -1){
			printf("New Conn!\n");
			SYS((childPid = fork()), "fork");
			if(childPid) {
				//parent
				continue;
			} else {
				//child
				int readChar;
				char buffer[SOCKET_BUFFER_SIZE];
				while((readChar = read(newConn, buffer, SOCKET_BUFFER_SIZE))!= 0){
					printf("%s", buffer);
					printf("\n");
				}
				printf("Closing connection\n");
				exit(0);
			}
		}
	}
	printf("Closing server.\n");
	close(socketFd);
	exit(0);
}
