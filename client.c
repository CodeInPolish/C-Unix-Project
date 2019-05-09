#include "global.h"
#include "sockets.h"
#include "fileIO.h"

#define BUFFER_SIZE 1024
void readConsoleCommand();
void commandAdd(int socketFd);
void commandRun(int programNumber, int socketFd);
void commandMultiRun();
void commandQuit(int socketFd);
char* getFilename(char* path);

int main(int argv, char** argc){
	if(argv != 4){
		printf("3 parameters needed. Got %d\n", argv-1);
		exit(1);
	}
	char* address = argc[1];
	int port = char2int(argc[2]);
	int delay = char2int(argc[3]);
	printf("addr: %s , port: %d, delay: %d\n", address, port, delay);

	int socketFd = setupClientSocket(SERVER_IP, port);
	printf("Socket id: %d \n", socketFd);
	while(1){
		readConsoleCommand(socketFd);
	}
	closeSocket(socketFd);
	exit(0);
}

void readConsoleCommand(int socketFd) {
	char command[2];
	read(STDIN, &command, sizeof(command));
	switch(command[0]) {
		case '+': 
			commandAdd(socketFd);
			break;
		case '*':
			commandMultiRun();
			break;
		case '@':
			commandRun(2, socketFd);
			break;
		case 'q':
			commandQuit(socketFd);
			break;
		default:
			printf("Commande non reconnue\n");
			break;
	}
}

void commandAdd(int socketFd) {
	char buffer[BUFFER_SIZE];
	char* path;
	int readChar = read(STDIN, &buffer, BUFFER_SIZE);
	if(buffer[readChar-1] == '\n'){
		path = malloc((readChar)*sizeof(char));
		strncpy(path, buffer, readChar-1);
	}
	char* fileName = getFilename(path);
	serverCommand cmd;
	memset(&cmd,0,sizeof(cmd));
	cmd.command = Add;
	cmd.programNameLength = strlen(fileName);
	strncpy(cmd.programName, fileName, cmd.programNameLength);
	send(socketFd, &cmd, sizeof(cmd), 0);
	readAndSendFile(path, socketFd, SOCKET_BUFFER_SIZE);
	closeSocketWrite(socketFd);
	free(path);
}

void commandRun(int programNumber, int socketFd) {

}

void commandMultiRun() {

}

void commandQuit(int socketFd) {
	closeSocket(socketFd);
	exit(0);
}

char* getFilename(char* path){
	int length = strlen(path);
	int i = length;
	while(i>0){
		if(path[i] == '/'){
			break;
		}
		i--;
	}
	if(i == 0){
		return path;
	}
	return path+i+1;
}