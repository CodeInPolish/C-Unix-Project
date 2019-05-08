#include "global.h"
#include "sockets.h"

#define BUFFER_SIZE 1024
void readConsoleCommand();
void commandAdd(char* path, int socketFd);
void commandRun(int programNumber, int socketFd);
void commandMultiRun();
void commandQuit(int socketFd);
void readAndSendFile(char* path, int socketFd);

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
		case '+': {
			char buffer[BUFFER_SIZE];
			int readChar = read(STDIN, &buffer, BUFFER_SIZE);
			if(buffer[readChar-1] == '\n'){
				char* path = malloc((readChar+1)*sizeof(char));
				strncpy(path, buffer, readChar);
				commandAdd(path, socketFd);
				free(path);
			}
			break;
				}
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

void commandAdd(char* path, int socketFd) {
	readAndSendFile(path, socketFd);
}

void commandRun(int programNumber, int socketFd) {

}

void commandMultiRun() {

}

void commandQuit(int socketFd) {
	closeSocket(socketFd);
	exit(0);
}

void readAndSendFile(char* path, int socketFd){
	int fileFd = open(path, O_RDONLY);
	int readChar;
	char buffer[SOCKET_BUFFER_SIZE];
	while((readChar = read(fileFd, buffer, SOCKET_BUFFER_SIZE)) != EOF){
		write(STDOUT, buffer, readChar);
		fflush(stdout);
		//send(socketFd, buffer, readChar, 0);
		if(buffer[readChar] == '\0'){
			break;
		}
	}
	close(fileFd);
	closeSocketWrite(socketFd);
}