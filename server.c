#include "global.h"
#include "sockets.h"
#include "fileIO.h"

#define SERVERPATH "./serverFiles/"

void addCommand(char* path, char* fileName, int socketFd);

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
				serverCommand cmd;
				read(newConn, &cmd, sizeof(cmd));
				switch(cmd.command){
					case Add:
						addCommand(SERVERPATH, cmd.programName, newConn);
						break;
					case Run:
						printf("Run cmd\n");
						break;
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

void addCommand(char* path, char* fileName, int socketFd){
	int len = strlen(path)+strlen(fileName);
	char* fullPath = SYSN(malloc((len+1)*sizeof(char)),"malloc error");
	strcpy(fullPath, path);
	strcpy(fullPath+strlen(path), fileName);
	receiveAndWrite(fullPath, socketFd, SOCKET_BUFFER_SIZE);
	//compile and other shit
	free(fullPath);
}
