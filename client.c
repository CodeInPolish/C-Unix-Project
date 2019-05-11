#include "global.h"
#include "sockets.h"
#include "fileIO.h"

#define BUFFER_SIZE 1024

void readConsoleCommand(char* address, int port, int delay);
void commandAdd(char* address, int port);
void commandRun(char* address, int port);
void commandMultiRun(char* address, int port, int delay);
void commandQuit();
void multiRun(char* address, int port, int programNumber);
void readServerResponse(int socketFd, int fd);
char* getFilename(char* path);
char* createString(int val1, int val2, int val3, int val4);

int main(int argv, char** argc){
	if(argv != 4){
		printf("3 parameters needed. Got %d\n", argv-1);
		exit(1);
	}
	char* address = argc[1];
	int port = char2int(argc[2]);
	int delay = char2int(argc[3]);
	while(1) {
		readConsoleCommand(address, port, delay);
	}
	exit(0);
}

void readConsoleCommand(char* address, int port, int delay) {
	char command[2];
	read(STDIN, &command, sizeof(command));
	switch(command[0]) {
		case '+': 
			commandAdd(address, port);
			break;
		case '*':
			commandMultiRun(address, port, delay);
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
	free(progNum);
	send(socketFd, &cmd, sizeof(cmd), 0);
	readServerResponse(socketFd, STDOUT);
}

void readServerResponse(int socketFd, int fd){
	serverResponse svrR;
	read(socketFd, &svrR, sizeof(svrR));
	char* str = createString(svrR.programNumber, svrR.programState, svrR.executionTime, svrR.returnCode);
	write(fd, str, strlen(str));
	if(svrR.programState == Normal){
		char buffer[SOCKET_BUFFER_SIZE];
		int readChar;
		while((readChar = read(socketFd, buffer, SOCKET_BUFFER_SIZE))>0){
			write(fd, buffer, readChar);
		}
	}
}
void commandMultiRun(char* address, int port, int delay) {
	char buffer[BUFFER_SIZE];
	char* progNum;
	int programNumber;
	int readChar = read(STDIN, &buffer, BUFFER_SIZE);
	if(buffer[readChar-1] == '\n'){
		progNum = malloc((readChar)*sizeof(char));
		programNumber = char2int(progNum);
	}
	int childPid;
	childPid = SYS(fork(),"fork error");
	if(childPid){
	} else {
		int delayPipe[2];
		SYS(pipe(delayPipe), "pipe error");
		int delayChild = SYS(fork(),"fork error");
		char buffer[4];
		if(delayChild){
			while(1){
				read(delayPipe[0], buffer, 4);
				multiRun(address, port, programNumber);
			}
		} else {
			while(1){
				write(delayPipe[1], "r", 4);
				sleep(delay);
			}
		}
	}
	free(progNum);
}

void commandQuit(pid_t child1, pid_t child2) {
	kill(0, SIGKILL);
	exit(0);
}

void multiRun(char* address, int port, int programNumber) {
	int socketFd = setupClientSocket(SERVER_IP, port);
	serverCommand cmd;
	memset(&cmd,0,sizeof(cmd));
	cmd.command = Run;
	cmd.programNumber = programNumber;
	send(socketFd, &cmd, sizeof(cmd), 0);
	readServerResponse(socketFd, STDOUT);
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

char* createString(int val1, int val2, int val3, int val4){
	char* str = SYSN(calloc(150, sizeof(char)),"calloc error");
	sprintf(str, "Program Number:%d, Status:%d, executionTime: %d, exitCode: %d\n",
		val1, val2, val3, val4);
	return str;
}