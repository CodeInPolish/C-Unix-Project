#include "global.h"
#include "sockets.h"
#include "fileIO.h"
#include "ipc.h"

#define SERVERPATH "./serverFiles/"
#define SERVERLOGS_COMPILE "./serverFiles/logs/compile/"
#define SERVERLOGS_EXECUTE "./serverFiles/logs/execute/"
#define COMPILERPATH "/usr/bin/cc"

void addCommand(char* path, char* fileName, int socketFd, Ressource* sharedMem);
void runCommand(int socketFd, int programNumber, Ressource*);
int compile(char* toCompile, char* logFullPath, char* logFileName, char* programNumber);
int getFreeSlot(Ressource* sharedMem);
char* getName(char* path);
char* concat(char* str1, char* str2);

int main(int argv, char** argc){

	if(argv != 2) {
		printf("1 parameter needed. Got %d\n", argv-1);
		exit(1);
	}
	
	Ressource* sharedMem = getMemory();
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
				close(newConn);
				continue;
			} else {
				//child
				serverCommand cmd;
				read(newConn, &cmd, sizeof(cmd));
				switch(cmd.command){
					case Add:
					addCommand(SERVERPATH, cmd.programName, newConn, sharedMem);
					break;
					case Run:
					runCommand(newConn, cmd.programNumber, sharedMem);
					break;
					default:
					closeSocket(newConn);
					printf("Cmd error\n");
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

void addCommand(char* path, char* fileName, int socketFd, Ressource* sharedMem){
	char* fullPath = concat(path, fileName);
	receiveAndWrite(fullPath, socketFd, SOCKET_BUFFER_SIZE);
	int programNum = getFreeSlot(sharedMem);
	char programNumber[4];
	sprintf(programNumber, "%d", programNum);
	char* logFileName = concat(programNumber, ".txt");
	char* logFullPath = concat(SERVERLOGS_COMPILE, logFileName);
	int compileFailed = compile(fullPath, logFullPath, logFileName, programNumber);
	serverCommand cmd;
	cmd.programNumber = programNum;
	send(socketFd, &cmd, sizeof(cmd), 0);
	if(compileFailed){
		readAndSendFile(logFullPath,socketFd, SOCKET_BUFFER_SIZE);
	}
	free(fullPath);
	free(logFileName);
	free(logFullPath);
	closeSocket(socketFd);
}

void runCommand(int socketFd, int programNum, Ressource* sharedMem){
	char programNumber[4];
	sprintf(programNumber, "%d", programNum);
	int status;
	pid_t childPid = SYS(fork(), "fork error");
	if(childPid == 0) {
		char* execPath = concat(SERVERPATH, programNumber);
		SYS(execl(execPath, programNumber, (char*)NULL),"exec error");
		printf("executed\n");
		free(execPath);
		exit(0);
	}
	waitpid(childPid, &status, 0);
}

int compile(char* toCompile, char* logFullPath, char* logFileName, char* programNumber){
	int fd = SYS(open(logFullPath, O_CREAT | O_WRONLY| O_TRUNC, 0666),"open error");
	int stderr_copy = SYS(dup(2),"dup error");
	SYS(dup2(fd, 2),"dup2 error");
	int status;
	pid_t childPid = SYS(fork(), "fork error");
	if(childPid == 0) {
		char* execPath = concat(SERVERPATH, programNumber);
		SYS(execl(COMPILERPATH,"cc",toCompile,"-o",execPath,(char*) NULL),"exec error");
		free(execPath);
		exit(0);
	}
	waitpid(childPid, &status, 0);
	SYS(dup2(stderr_copy, 2), "dup2 error");
	close(stderr_copy);
	close(fd);
	return WEXITSTATUS(status);
}

char* getName(char* path){
	int length = strlen(path);
	int i = length;
	while(i>0){
		if(path[i] == '.'){
			break;
		}
		i--;
	}
	if(i == 0){
		return path;
	}
	return path+i+1;
}

char* concat(char* str1, char* str2){
	int len = strlen(str1)+strlen(str2);
	char* fullPath = SYSN(calloc((len+1), sizeof(char)),"calloc error");
	strcpy(fullPath, str1);
	strcpy(fullPath+strlen(str1), str2);
	return fullPath;
}

int getFreeSlot(Ressource* sharedMem){
	down(0);
	int i = 0;
	while(i<1000){
		if(sharedMem[i].isFree){
			sharedMem[i].isFree = 0;
			up(0);
			return i;
		}
	}
	up(0);
	return -1;
}