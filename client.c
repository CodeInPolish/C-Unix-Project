#include "global.h"
#include "sockets.h"
#include "fileIO.h"

#define BUFFER_SIZE 1024
#define pipeDelayIn pipeDelay[1]
#define pipeDelayOut pipeDelay[0]
#define pipeChildRunIn pipeChildRun[1]
#define pipeChildRunOut pipeChildRun[0]
#define pipeChildRespIn pipeChildResp[1]
#define pipeChildRespOut pipeChildResp[2]

void readConsoleCommand(char* address, int port, int pipeIn, int pipeOut);
void commandAdd(char* address, int port);
void commandRun(char* address, int port);
void commandMultiRun();
void commandQuit();
char* getFilename(char* path);

int main(int argv, char** argc){
	if(argv != 4){
		printf("3 parameters needed. Got %d\n", argv-1);
		exit(1);
	}
	int pipeDelay[2], pipeChildRun[2], pipeChildResp[2];
	SYS(pipe(pipeDelay), "pipe error");
	SYS(pipe(pipeChildRun), "pipe error");
	SYS(pipe(pipeChildResp), "pipe error");
	char* address = argc[1];
	int port = char2int(argc[2]);
	int delay = char2int(argc[3]);
	int delayChild = SYS(fork(), "fork error");
	if(delayChild) {
		// parent
		int multiRunChild = SYS(fork(),"fork error");

		if(multiRunChild) {
			//parent
			close(pipeDelayIn);
			close(pipeDelayOut);
			close(pipeChildRespIn);
			close(pipeChildRunOut);
			while(1){
				readConsoleCommand(address, port, pipeChildRunIn, pipeChildRespOut);
			}
			close(pipeChildRespOut);
			close(pipeChildRunIn);
		} else {
			//multiRun child
			close(pipeDelayIn);
			close(pipeChildRunIn);
			close(pipeChildRespOut);
			while(1){
				char buffer[4];
				read(pipeDelayOut, &buffer, sizeof(buffer));
				commandMultiRun(address, port, 0);
			}
			close(pipeDelayOut);
			close(pipeChildRunOut);
			close(pipeChildRespIn);
		}
	} else {
		//delayChild
		close(pipeChildRespIn);
		close(pipeChildRespOut);
		close(pipeChildRunIn);
		close(pipeChildRunOut);
		close(pipeDelayOut);
		while(1){
			sleep(delay);
			//write(pipeDelayIn, "run", 3);
		}
		close(pipeDelayIn);
	}
	exit(0);
}

void readConsoleCommand(char* address, int port, int pipeIn, int pipeOut) {
	char command[2];
	read(STDIN, &command, sizeof(command));
	switch(command[0]) {
		case '+': 
			commandAdd(address, port);
			break;
		case '*':
			commandMultiRun();
			break;
		case '@':
			commandRun(address, port);
			break;
		case 'q':
			commandQuit();
			break;
		default:
			printf("Commande non reconnue\n");
			break;
	}
}

void commandAdd(char* address, int port) {
	int socketFd = setupClientSocket(SERVER_IP, port);
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

	read(socketFd, &cmd, sizeof(cmd));
	printf("Program number: %d\n", cmd.programNumber);
	while((readChar = read(socketFd, &buffer, BUFFER_SIZE)) > 0){
		write(STDOUT, buffer, readChar);
		fflush(stdout);
	}
	write(STDOUT, "\n", 1);
	close(socketFd);
}

void commandRun(char* address, int port) {
	char buffer[BUFFER_SIZE];
	char* progNum;
	int readChar = read(STDIN, &buffer, BUFFER_SIZE);
	if(buffer[readChar-1] == '\n'){
		progNum = malloc((readChar)*sizeof(char));
		strncpy(progNum, buffer, readChar-1);
	}
	int socketFd = setupClientSocket(SERVER_IP, port);
	serverCommand cmd;
	memset(&cmd,0,sizeof(cmd));
	cmd.command = Run;
	cmd.programNumber = char2int(progNum);
	send(socketFd, &cmd, sizeof(cmd), 0);
	serverResponse svrR;
	read(socketFd, &svrR, sizeof(svrR));
	printf("Program Number:%d, Status:%d, executionTime: %d, exitCode: %d\n", 
		svrR.programNumber, svrR.programState, svrR.executionTime, svrR.returnCode);
	if(svrR.programState == Normal){
		char buffer[SOCKET_BUFFER_SIZE];
		int readChar;
		while((readChar = read(socketFd, buffer, SOCKET_BUFFER_SIZE))>0){
			printf("%s", buffer);
		}
	}
}

void commandMultiRun(char* address, int port, int programNumber) {
	int socketFd = setupClientSocket(SERVER_IP, port);
	serverCommand cmd;
	memset(&cmd,0,sizeof(cmd));
	cmd.command = Run;
	cmd.programNumber = programNumber;
	send(socketFd, &cmd, sizeof(cmd), 0);
}

void commandQuit() {
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