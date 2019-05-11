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
void runLogic(int socketFd, int programNumber, Ressource* sharedMem, char* programNum);
int run(char* programNum);
int getFreeSlot(Ressource* sharedMem);
char* getName(char* path);
char* concat(char* str1, char* str2);
void writeSharedMem(Ressource* sharedMem, int programNumber, char* programName, int compile);
void writeSharedMem2(Ressource* sharedMem, int programNumber, long time);
long now();

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
				closeSocket(newConn);
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
		readAndSendFile(logFullPath, socketFd, SOCKET_BUFFER_SIZE);
		writeSharedMem(sharedMem, programNum, fileName, 0);
	} else {
		writeSharedMem(sharedMem, programNum, fileName, 1);
	}
	free(fullPath);
	free(logFileName);
	free(logFullPath);
}

void runCommand(int socketFd, int programNum, Ressource* sharedMem){
	char programNumber[4];
	sprintf(programNumber, "%d", programNum);
	runLogic(socketFd, programNum, sharedMem, programNumber);
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

void runLogic(int socketFd, int programNum, Ressource* sharedMem, char* programNumber){
	int compile, notExists;

	down(0);
	compile = sharedMem[programNum].compile;
	notExists = sharedMem[programNum].isFree; 
	up(0);

	serverResponse svrR;
	svrR.programNumber = programNum;
	if(notExists){
		svrR.programState = NotExist;
		send(socketFd, &svrR, sizeof(svrR), 0);
	} else if (compile == 0){
		svrR.programState = NotCompile;
		send(socketFd, &svrR, sizeof(svrR), 0);
	} else {
		long t1 = now();
		int exitCode = run(programNumber);
		long t2 = now();
		if(exitCode) {
			svrR.programState = RuntimeError;
			svrR.returnCode = exitCode;
			send(socketFd, &svrR, sizeof(svrR), 0);
		} else {
			svrR.executionTime = t2-t1;
			svrR.programState = Normal;
			svrR.returnCode = exitCode;
			send(socketFd, &svrR, sizeof(svrR), 0);
			char* execFileName = concat(programNumber, ".txt");
			char* execFullPath = concat(SERVERLOGS_EXECUTE, execFileName);
			readAndSendFile(execFullPath, socketFd, SOCKET_BUFFER_SIZE);
			free(execFullPath);
			free(execFileName);
		}
		writeSharedMem2(sharedMem, programNum, t2-t1);
	}
}

int run(char* programNum){
	int status;
	char* execFileName = concat(programNum, ".txt");
	char* execFullPath = concat(SERVERLOGS_EXECUTE, execFileName);
	int fd = SYS(open(execFullPath, O_CREAT | O_WRONLY| O_TRUNC, 0666),"open error");
	int stdout_copy = SYS(dup(1),"dup error");
	SYS(dup2(fd, 1),"dup2 error");
	pid_t childPid = SYS(fork(), "fork error");
	if(childPid == 0) {
		char* execPath = concat(SERVERPATH, programNum);
		SYS(execl(execPath, programNum, (char*)NULL),"exec error");
		free(execPath);
		exit(0);
	}
	waitpid(childPid, &status, 0);
	printf("child status:%d\n", status);
	free(execFileName);
	free(execFullPath);
	SYS(dup2(stdout_copy, 1), "dup2 error");
	close(stdout_copy);
	close(fd);
	return status;
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
		i++;
	}
	up(0);
	return -1;
}

void writeSharedMem(Ressource* sharedMem, int programNumber, char* programName, int compile){
	down(0);
	sharedMem[programNumber].number = programNumber;
	strcpy(sharedMem[programNumber].name, programName);
	sharedMem[programNumber].compile = compile;
	up(0);
}

void writeSharedMem2(Ressource* sharedMem, int programNumber, long time){
	down(0);
	sharedMem[programNumber].execNumber++;
	sharedMem[programNumber].execTime += time;
	up(0);
}

long now(){
	struct timeval  tv;
	SYS(gettimeofday(&tv, NULL),"gettimeofday error");
	return tv.tv_sec * 1000000 + tv.tv_usec;
}